<?php

/**
 * @file
 * Default Synonyms Extractor class that ships together with the Synonym module.
 */

class SynonymsSynonymsExtractor extends AbstractSynonymsExtractor {

  static public function fieldTypesSupported() {
    return array('text', 'number_integer', 'number_float', 'number_decimal');
  }

  static public function synonymsExtract($items, $field, $instance, $entity, $entity_type) {
    $synonyms = array();

    foreach ($items as $item) {
      $synonyms[] = $item['value'];
    }

    return $synonyms;
  }

  static public function processEntityFieldQuery($tag, EntityFieldQuery $query, $field, $instance) {
    $query->fieldCondition($field, 'value', '%' . $tag . '%', 'LIKE');
  }
}
