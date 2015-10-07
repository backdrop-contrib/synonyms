<?php

/**
 * @file
 * Enables Taxonomy Term Reference field type to be source of synonyms.
 */

/**
 * Definition of TaxonomySynonymsBehavior class.
 */
class TaxonomySynonymsBehavior implements SynonymsSynonymsBehavior, AutocompleteSynonymsBehavior, SearchSynonymsBehavior {

  public function extractSynonyms($items, $field, $instance, $entity, $entity_type) {
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

    $efq = $this->targetTermEntityFieldQuery($field, $instance);
    $efq->propertyCondition('name', '%' . $tag . '%', 'LIKE');
    $result = $efq->execute();
    if (isset($result['taxonomy_term'])) {
      $tids = array_keys($result['taxonomy_term']);
    }

    // Now we have tids of terms from the referenced vocabulary which names
    // LIKE %$tag%, suggested are the terms that refer to any of these $tids.
    if (empty($tids)) {
      return FALSE;
    }
    $query->fieldCondition($field, 'tid', $tids);

    return TRUE;
  }

  public function mergeEntityAsSynonym($items, $field, $instance, $synonym_entity, $synonym_entity_type) {
    // Taxonomy term reference supports only referencing of entity types
    // 'taxonomy_term'.. duh.
    if ($synonym_entity_type != 'taxonomy_term') {
      return array();
    }
    // Checking that $field is configured to reference the vocabulary of
    // $synonym_entity term.
    $is_allowed = FALSE;
    foreach ($field['settings']['allowed_values'] as $setting) {
      if ($synonym_entity->vocabulary_machine_name == $setting['vocabulary']) {
        if ($setting['parent'] == 0) {
          // No need to check parent property as there is no limitation on it.
          $is_allowed = TRUE;
          break;
        }
        else {
          foreach (taxonomy_get_parents_all($synonym_entity->tid) as $parent) {
            if ($parent->tid == $setting['parent']) {
              $is_allowed = TRUE;
              break(2);
            }
          }
        }
      }
    }
    if (!$is_allowed) {
      // Synonym term is from a vocabulary that is not expected by this field,
      // or under unexpected parent.
      return array();
    }
    return array(array(
      'tid' => $synonym_entity->tid,
    ));
  }

  public function synonymItemHash($item, $field, $instance) {
    return $item['tid'];
  }

  public function synonymFind($synonym, $field, $instance) {
    $efq = $this->targetTermEntityFieldQuery($field, $instance);
    $efq->propertyCondition('name', $synonym);
    $result = $efq->execute();
    if (isset($result['taxonomy_term'])) {
      $efq = new EntityFieldQuery();
      $efq->entityCondition('entity_type', $instance['entity_type']);
      $efq->entityCondition('bundle', $instance['bundle']);
      $efq->fieldCondition($field, 'tid', array_keys($result['taxonomy_term']));
      $result = $efq->execute();
      if (isset($result[$instance['entity_type']])) {
        return array_keys($result[$instance['entity_type']]);
      }
    }
    return array();
  }

  /**
   * Supportive method to query the referenced terms through provided field.
   *
   * Prepare an EntityFieldQuery to query the terms of the provided term
   * reference field.
   *
   * @param array $field
   *   Field API field definition array of the term reference field, based on
   *   which the EntityFieldQuery object should be built
   * @param array $instance
   *   Field API instance definition array of the term reference field, based
   *   on which the EntityFieldQuery object should be built
   *
   * @return EntityFieldQuery
   */
  protected function targetTermEntityFieldQuery($field, $instance) {
    $efq = new EntityFieldQuery();
    $efq->entityCondition('entity_type', 'taxonomy_term');

    if (isset($field['settings']['allowed_values']) && is_array($field['settings']['allowed_values'])) {
      $bundles = array();
      foreach ($field['settings']['allowed_values'] as $v) {
        $bundles[] = $v['vocabulary'];
      }
      $efq->entityCondition('bundle', $bundles, 'IN');
    }

    return $efq;
  }
}
