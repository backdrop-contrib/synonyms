<?php

/**
 * @file
 * Documentation for Synonyms Provider Field module.
 */


/**
 * Hook to collect info about available field-based synonym providers.
 *
 * Hook to collect info about what PHP classes implement provided synonyms
 * behavior for different field types. If you to create synonyms provider backed
 * by some field type, this hook is for you. Information from this hook will be
 * post-processed then based on existing fields and instances and then inserted
 * into hook_synonyms_provider_info().
 *
 * @param string $behavior
 *   Name of a synonyms behavior. This string will always be among the keys
 *   of the return of synonyms_behaviors(), i.e. name of a cTools plugin
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
function hook_synonyms_field_provider_info($behavior) {
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
 * Hook to alter info about available field-based synonyms providers.
 *
 * This hook is invoked right after
 * hook_synonyms_provider_field_synonyms_provider_field_provider_info() and is
 * designed to let modules overwrite implementation info from some other
 * modules. For example, if module A provides implementation for some field
 * type, but your module has a better version of that implementation, you would
 * need to implement this hook and to overwrite the implementation info.
 *
 * @param array $field_providers
 *   Array of information about existing field-based synonyms providers that was
 *   collected from modules
 * @param string $behavior
 *   Name of the behavior for which the field-based synonyms providers are being
 *   generated
 */
function hook_synonyms_field_provider_info_alter(&$field_providers, $behavior) {
  switch ($behavior) {
    case 'the-behavior-i-want':
      $field_providers['the-field-type-i-want'] = 'MyFieldTypeAutocompleteSynonymsBehavior';
      break;
  }
}
