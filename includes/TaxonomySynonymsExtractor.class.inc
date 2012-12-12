<?php

/**
 * @file
 * Enables Taxonomy Term Reference field types to be source of synonyms.
 */

class TaxonomySynonymsExtractor extends AbstractSynonymsExtractor {

  public function fieldTypesSupported() {
    return array('taxonomy_term_reference');
  }

  public function synonymsExtract($items, $field, $instance, $entity, $entity_type) {
    $synonyms = array();

    $terms = array();
    foreach ($items as $item) {
      $terms[] = $item['tid'];
    }
    $terms = taxonomy_term_load_multiple($terms);
    foreach ($terms as $term) {
      $synonyms[] = entity_label('taxonomy_term', $term);
    }
    return $synonyms;
  }

  public function processEntityFieldQuery($tag, EntityFieldQuery $query, $field, $instance) {
    // Unfortunately EntityFieldQuery does not currently support INNER JOINing
    // term entity that is referenced via taxonomy_term_reference field type.
    // Thus, we use an ugly solution -- going through all terms that exist in
    // vocabulary and trying to see if there is a match by term's name.
    $tids = array();

    foreach ($field['settings']['allowed_values'] as $settings) {
      $efd = new EntityFieldQuery();
      $efd->entityCondition('bundle', $settings['vocabulary'])
        ->entityCondition('entity_type', 'taxonomy_term')
        ->propertyCondition('name', '%' . $tag . '%', 'LIKE');
      $result = $efd->execute();
      if (isset($result['taxonomy_term'])) {
        foreach ($result['taxonomy_term'] as $tid) {
          $tids[] = $tid->tid;
        }
      }
    }

    // Now we have tids of terms from the referenced vocabulary which names
    // LIKE %$tag%, suggested are the terms that refer to any of these $tids.
    if (empty($tids)) {
      // No possible suggestions were found. We have to make sure $query yields
      // no results.
      $this->emptyResultsCondition($query);
      return;
    }
    $query->fieldCondition($field, 'tid', $tids);
  }
}
