<?php

/**
 * @file
 * Default Synonyms Extractor class that ships together with the Synonym module.
 */

class SynonymsSynonymsExtractor extends AbstractSynonymsExtractor {

  public static function fieldTypesSupported() {
    return array('text', 'number_integer', 'number_float', 'number_decimal');
  }

  public static function synonymsExtract($items, $field, $instance, $entity, $entity_type) {
    $synonyms = array();

    foreach ($items as $item) {
      $synonyms[] = $item['value'];
    }

    return $synonyms;
  }

  public static function processEntityFieldQuery($tag, EntityFieldQuery $query, $field, $instance) {
    $query->fieldCondition($field, 'value', '%' . $tag . '%', 'LIKE');
  }

  public static function mergeEntityAsSynonym($items, $field, $instance, $synonym_entity, $synonym_entity_type) {
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

  public static function synonymFind($synonym, $field, $instance, $entity_type, $bundle = NULL) {
    $efq = new EntityFieldQuery();
    $efq->entityCondition('entity_type', $entity_type);
    if ($bundle) {
      $efq->entityCondition('bundle', $bundle);
    }
    $efq->fieldCondition($field, 'value', $synonym);
    $result = $efq->execute();
    return isset($result[$entity_type]) ? array_keys($result[$entity_type]) : array();
  }
}
