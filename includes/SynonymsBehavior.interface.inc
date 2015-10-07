<?php

/**
 * @file
 * Interfaces of synonyms behaviors that are shipped with Synonyms module.
 */

/**
 * General interface of a synonyms behavior.
 *
 * All synonyms behaviors must extend this interface.
 */
interface SynonymsBehavior {
}

/**
 * Interface of "synonyms" behavior.
 *
 * The most most basic synonyms behavior.
 */
interface SynonymsSynonymsBehavior extends SynonymsBehavior {

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
  public function extractSynonyms($items, $field, $instance, $entity, $entity_type);

  /**
   * Add an entity as a synonym into a field of another entity.
   *
   * Basically this method should be called when you want to add some entity
   * as a synonym to another entity (for example when you merge one entity
   * into another and besides merging want to add synonym of the merged entity
   * into the trunk entity). You should extract synonym value (according to what
   * value is expected in this field) and return it. We try to provide you with
   * as much of context as possible, but normally you would only need
   * $synonym_entity and $synonym_entity_type parameters. Return an empty array
   * if entity of type $synonym_entity_type cannot be converted into a format
   * expected by $field.
   *
   * @param array $items
   *   Array items that already exist in the field into which new synonyms is to
   *   be added
   * @param array $field
   *   Field array definition according to Field API of the field into which new
   *   synonym is to be added
   * @param array $instance
   *   Instance array definition according to Field API of the instance into
   *   which new synonym is to be added
   * @param object $synonym_entity
   *   Fully loaded entity object which has to be added as synonym
   * @param string $synonym_entity_type
   *   Entity type of $synonym_entity
   *
   * @return array
   *   Array of extra items to be merged into the items that already exist in
   *   field values
   */
  public function mergeEntityAsSynonym($items, $field, $instance, $synonym_entity, $synonym_entity_type);

  /**
   * Hash a field item that is enabled as synonym.
   *
   * Your hash function must return such hash that for 2 items that yield the
   * same synonyms their hash must be the same. There is no limit on minimal or
   * maximum hash length, but keep it reasonable, something below 512 symbols.
   * Also, your hash function should strive to minimize hash collisions, i.e.
   * when 2 different items yield the same hash.
   *
   * @param array $item
   *   Field item whose hash is requested
   * @param array $field
   *   Field from which the $item comes from
   * @param array $instance
   *   Instance from which the $item comes from
   *
   * @return string
   *   Hash of the provided $item
   */
  public function synonymItemHash($item, $field, $instance);

  /**
   * Search whether there is a provided synonym stored in a provided field.
   *
   * Determine if there are any entities that have the provided $synonym as
   * their synonym particularly stored within the provided field.
   *
   * @param string $synonym
   *   What synonym should be sought for
   * @param array $field
   *   Field API field definition array of the field within which the search
   *   for synonym should be performed
   * @param array $instance
   *   Field API instance definition array of the instance within which the
   *   search for synonym should be performed
   *
   * @return array
   *   An array of entity IDs that have $synonym as their synonym stored in the
   *   provided field
   */
  public function synonymFind($synonym, $field, $instance);

  // TODO: move this method back into AutocompleteSynonymsBehavior where it
  // belongs.
  /**
   * Allow you to include entities for autocomplete suggestion that are possible
   * candidates based on your field as a source of synonyms. This method should
   * alter $query by adding your condition to it. Additionally, you are expected
   * to return either TRUE or FALSE, which either allow or disallow execution
   * of the $query (you may want to disallow its execution if you know
   * beforehand that it won't yield any result and thus saving execution of a
   * useless SELECT).
   *
   * @param string $tag
   *   What user has typed in into autocomplete widget. Normally you would
   *   run LIKE '%$tag%' on your column
   * @param EntityFieldQuery $query
   *   EntityFieldQuery object where you should put your conditions to
   * @param array $field
   *   Array of field definition according to Field API
   * @param array $instance
   *   Array of instance definition according to Field API
   *
   * @return bool
   *   Whether to execute the $query. You may want to call its execution off, if
   *   you know the query won't yield any result set anyway.
   */
  public function processEntityFieldQuery($tag, EntityFieldQuery $query, $field, $instance);
}

/**
 * Interface of search integration synonyms behavior.
 */
interface SearchSynonymsBehavior extends SynonymsSynonymsBehavior {
}

/**
 * Interface of the autocomplete synonyms behavior.
 */
interface AutocompleteSynonymsBehavior extends SynonymsSynonymsBehavior {
}
