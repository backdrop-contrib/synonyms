<?php

/**
 * @file
 * Plugin definition for autocomplete synonyms behavior.
 */

$plugin = array(
  'title' => t('Autocomplete'),
  'description' => t('Synonyms friendly autocomplete'),
  'settings form callback' => 'synonyms_behavior_autocomplete_settings_form',
  'interface' => 'AutocompleteSynonymsBehavior',
);

/**
 * Settings form for autocomplete behavior.
 */
function synonyms_behavior_autocomplete_settings_form($form, &$form_state, $settings) {
  $element = array();

  $element['wording'] = array(
    '#type' => 'textfield',
    '#title' => t('Autocomplete wording'),
    // TODO: make it happen, enable this wording setting.
    '#default_value' => 'Coming soon', //isset($settings['wording']) ? $settings['wording'] : '@synonym is a synonym of @term',
    '#description' => t('Specify with what wording the synonyms should be suggested in the autocomplete feature. You may use <em>@synonym</em> and <em>@term</em> as corresponding placeholders.'),
    '#required' => TRUE,
    '#disabled' => TRUE,
  );

  return $element;
}

