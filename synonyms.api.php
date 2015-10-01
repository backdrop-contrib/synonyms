<?php

/**
 * @file
 * Documentation for Synonyms module.
 */

/**
 * Hook to collect info about available synonyms behavior implementations.
 *
 * Hook to collect info about what PHP classes implement provided synonyms
 * behavior for different field types.
 *
 * @param string $behavior
 *   Name of a synonyms behavior. This string will always be among the keys
 *   of the return of synonyms_behaviors(), i.e. name of a ctools plugin
 *
 * @return array
 *   Array of information about what synonyms behavior implementations your
 *   module supplies. The return array must contain field types as keys, whereas
 *   corresponding values should be names of PHP classes that implement the
 *   provided behavior for that field type. Read more about how to implement a
 *   specific behavior in the advanced help of this module. In a few words: you
 *   will have to implement an interface that is defined in the behavior
 *   definition. Do not forget to make sure your PHP class is visible to Drupal
 *   auto discovery mechanism
 */
function hook_synonyms_behavior_implementation_info($behavior) {
  switch ($behavior) {
    case 'autocomplete':
      return array(
        'my-field-type' => 'MyFieldTypeAutocompleteSynonymsBehavior',
      );
      break;

    case 'another-behavior':
      return array(
        'my-field-type-or-yet-another-field-type' => 'MyFieldTypeAnotherBehaviorSynonymsBehavior',
      );
      break;
  }

  return array();
}

/**
 * Hook to alter info about available synonyms behavior implementations.
 *
 * This hook is invoked right after hook_synonyms_behavior_implementation_info()
 * and is designed to let modules overwrite implementation info from some other
 * modules. For example, if module A provides implementation for some field
 * type, but your module has a better version of that implementation, you would
 * need to implement this hook and to overwrite the implementation info.
 *
 * @param array $info
 *   Array of information about existing synonyms behavior implementations that
 *   was collected from modules
 * @param string $behavior
 *   Name of the behavior for which the info about implementation is being
 *   generated
 */
function hook_synonyms_behavior_implementation_info_alter(&$info, $behavior) {
  switch ($behavior) {
    case 'the-behavior-i-want':
      $info['the-field-type-i-want'] = 'MyFieldTypeAutocompleteSynonymsBehavior';
      break;
  }
}

/**
 * Example of how to implement a synonyms behavior for an arbitrary field type.
 */
class MyFieldTypeAutocompleteSynonymsBehavior implements AutocompleteSynonymsBehavior {

  public function extractSynonyms($items, $field, $instance, $entity, $entity_type) {
    // Let's say our synonyms is stored in the 'foo' column of the field.
    $synonyms = array();
    foreach ($items as $item) {
      $synonyms[] = $item['foo'];
    }
    return $synonyms;
  }

  public function mergeEntityAsSynonym($items, $field, $instance, $synonym_entity, $synonym_entity_type) {
    // Let's say we keep the synonyms as strings and under the 'foo' column, to
    // keep it consistent with the extractSynonyms() method.
    $label = entity_label($synonym_entity_type, $synonym_entity);
    return array(array(
      'foo' => $label,
    ));
  }

  public function processEntityFieldQuery($tag, EntityFieldQuery $query, $field, $instance) {
    // So we want to find such items in our field that begin with "$tag" string.
    $query->fieldCondition($field, 'foo', $tag . '%');

    // If we for some reason know beforehand that there will be no match in our
    // field, in order to not run the useless query, we can return FALSE here
    // and the query will not be executed.
    return FALSE;
  }

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
   * @param string $entity_type
   *   Among synonyms of what entity type to conduct the search. In other words,
   *   only synonyms of this entity type will be considered when searching for
   *   a match
   * @param string $bundle
   *   Optional argument to restrict the search not only by entity type but
   *   also on the bundle level
   *
   * @return array
   *   An array of entity IDs that have $synonym as their synonym stored in the
   *   provided field
   */
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
