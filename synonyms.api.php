<?php

/**
 * @file
 * Documentation for Synonyms module.
 */

/**
 * Provide Synonyms module with names of synonyms extractor classes.
 *
 * Provide Synonyms module with names of classes that are able to extract
 * synonyms from fields. Each of the provided classes should extend
 * AbstractSynonymsExtractor base class.
 *
 * @return array
 *   Array of strings, where each value is a name of synonyms extractor class
 */
function hook_synonyms_extractor_info() {
  return array(
    // Please see below the defintion of ApiSynonymsSynonymsExtractor class
    // for your reference.
    'ApiSynonymsSynonymsExtractor',
  );
}

/**
 * Dummy synonyms extractor class for documentation purposes.
 *
 * This is a copy of SynonymsSynonymsExtractor class providing an example of
 * how to write your own synonyms extractor class. See the definition of
 * AbstractSynonymsExtractor for reference and code comments. For more
 * complicated examples look at EntityReferenceSynonymsExtractor class.
 */
class ApiSynonymsSynonymsExtractor extends AbstractSynonymsExtractor {

  /**
   * @return array
   *   Array of Field API field types from which this class is able to extract
   *   synonyms
   */
  static public function fieldTypesSupported() {
    return array('text', 'number_integer', 'number_float', 'number_decimal');
  }

  /**
   * Extract synonyms from a field attached to an entity.
   *
   * We try to pass as many info about context as possible, however, normally
   * you will only need $items to extract the synonyms.
   *
   * @param array $items
   *   Array of items
   * @param array $field
   *   Array of field definition according to Field API
   * @param array $instance
   *   Array of instance definition according to Field API
   * @param object $entity
   *   Fully loaded entity object to which the $field and $instance with $item
   *   values is attached to
   * @param string $entity_type
   *   Type of the entity $entity according to Field API definition of entity
   *   types
   *
   * @return array
   *   Array of synonyms extracted from $items
   */
  static public function synonymsExtract($items, $field, $instance, $entity, $entity_type) {
    $synonyms = array();

    foreach ($items as $item) {
      $synonyms[] = $item['value'];
    }

    return $synonyms;
  }

  /**
   * Allow you to hook in during autocomplete suggestions generation.
   *
   * Allow you to include entities for autocomplete suggestion that are possible
   * candidates based on your field as a source of synonyms. This method is
   * void, however, you have to alter and add your condition to $query
   * parameter.
   *
   * @param string $tag
   *   What user has typed in into autocomplete widget. Normally you would
   *   run LIKE '%$tag%' on your column
   * @param EntityFieldQuery $query
   *   EntityFieldQuery object where you should add your conditions to
   * @param array $field
   *   Array of field definition according to Field API, autocomplete on which
   *   is fired
   * @param array $instance
   *   Array of instance definition according to Field API, autocomplete on
   *   which is fired
   */
  static public function processEntityFieldQuery($tag, EntityFieldQuery $query, $field, $instance) {
    $query->fieldCondition($field, 'value', '%' . $tag . '%', 'LIKE');
  }
}
