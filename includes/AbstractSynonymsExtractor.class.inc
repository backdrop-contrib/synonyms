<?php

/**
 * @file
 * Define interface required for extracting synonyms from field types.
 */

abstract class AbstractSynonymsExtractor {

  /**
   * @return array
   *   Array of Field API field types from which this class is able to extract
   *   synonyms
   */
  static public abstract function fieldTypesSupported();

  /**
   * Extract synonyms from a field attached to an entity.
   *
   * We try to pass as many info about context as possible, however, normally
   *   you will only need $items to extract the synonyms.
   *
   * @param array $items
   *   Array of items
   * @param array $field
   *   Array of field definition according to Field API
   * @param array $instance
   *   Array of instance definition according to Field API
   * @param object $entity
   *   Fully loaded entity object to which the $field and $instance with $item
   *     values is attached to
   * @param string $entity_type
   *   Type of the entity $entity according to Field API definition of entity
   *     types
   *
   * @return array
   *   Array of synonyms extracted from $items
   */
  static public abstract function synonymsExtract($items, $field, $instance, $entity, $entity_type);

  /**
   * Allow you to hook in during autocomplete suggestions generation.
   *
   * Allow you to include entities for autocomplete suggestion that are possible
   *   candidates based on your field as a source of synonyms. This method is
   *   void, however, you have to alter and add your condition to $query
   *   parameter.
   *
   * @param string $tag
   *   What user has typed in into autocomplete widget. Normally you would
   *     run LIKE '%$tag%' on your column
   * @param EntityFieldQuery $query
   *   EntityFieldQuery object where you should put your conditions to
   * @param array $field
   *   Array of field definition according to Field API
   * @param array $instance
   *   Array of instance definition according to Field API
   */
  static public abstract function processEntityFieldQuery($tag, EntityFieldQuery $query, $field, $instance);

  /**
   * Supportive method.
   *
   * Set such a condition on $query that it will always yield no results. Should
   *   be called from $this->processEntityFieldQuery() when for whatever reason
   *   the object can't alter $query to include matched synonyms. As a fallback
   *   it should call this method to make sure it filtered everything out.
   *
   * @param EntityFieldQuery $query
   *   Query object passed to $this->processEntityFieldQuery() method
   * @param array $field
   *   Field array passed to $this->processEntityFieldQuery() method
   */
  static protected function emptyResultsCondition(EntityFieldQuery $query) {
    $query->entityCondition('entity_id', -1);
  }
}
