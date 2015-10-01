<?php

/**
 * @file
 * Plugin definition for synonyms search behavior.
 */

$plugin = array(
  'title' => t('Search'),
  'description' => t('Integrate synonyms with Search module'),
  'interface' => 'SearchSynonymsBehavior',
  'enabled callback' => 'synonyms_behavior_search_enabled_disabled',
  'disabled callback' => 'synonyms_behavior_search_enabled_disabled',
);

/**
 * Callback for when the behavior is enabled/disabled.
 *
 * Trigger re-indexing of all the nodes that reference terms from the vocabulary
 * where the change has taken place.
 */
function synonyms_behavior_search_enabled_disabled($behavior_definition, $behavior_implementation, $instance) {
  module_load_include('inc', 'synonyms', 'synonyms.pages');
  synonyms_reindex_nodes_by_vocabulary(taxonomy_vocabulary_machine_name_load($instance['bundle']));
}
