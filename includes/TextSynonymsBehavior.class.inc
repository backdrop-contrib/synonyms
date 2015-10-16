<?php

/**
 * @file
 * Default Synonyms Extractor class that ships together with the Synonym module.
 */

/**
 * Definition of TextSynonymsBehavior class.
 */
class TextSynonymsBehavior implements SynonymsSynonymsBehavior, AutocompleteSynonymsBehavior, SelectSynonymsBehavior, SearchSynonymsBehavior {

  public function extractSynonyms($items, $field, $instance, $entity, $entity_type) {
    $synonyms = array();

    foreach ($items as $item) {
      $synonyms[] = $item['value'];
    }

    return $synonyms;
  }

  public function processEntityFieldQuery($tag, EntityFieldQuery $query, $field, $instance) {
    $query->fieldCondition($field, 'value', '%' . $tag . '%', 'LIKE');
    return TRUE;
  }

  public function mergeEntityAsSynonym($items, $field, $instance, $synonym_entity, $synonym_entity_type) {
    $synonym = entity_label($synonym_entity_type, $synonym_entity);
    switch ($field['type']) {
      case 'text':
        break;

      // We add synonyms for numbers only if $synonym is a number.
      case 'number_integer':
      case 'number_float':
      case 'number_decimal':
        if (!is_numeric($synonym)) {
          return array();
        }
        break;

    }
    return array(array(
      'value' => $synonym,
    ));
  }

  public function synonymItemHash($item, $field, $instance) {
    return $item['value'];
  }

  public function synonymFind($synonym, $field, $instance) {
    $efq = new EntityFieldQuery();
    $efq->entityCondition('entity_type', $instance['entity_type']);
    $efq->entityCondition('bundle', $instance['bundle']);
    $efq->fieldCondition($field, 'value', $synonym);
    $result = $efq->execute();
    return isset($result[$instance['entity_type']]) ? array_keys($result[$instance['entity_type']]) : array();
  }
}
