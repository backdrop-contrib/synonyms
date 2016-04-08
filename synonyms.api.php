<?php

/**
 * @file
 * Documentation for Synonyms module.
 */

/**
 * Collect info about available synonyms providers.
 *
 * If you module ships a synonyms provider you probably want to implement this
 * hook. However, exercise caution, if your synonyms provider is a field-based
 * one, you might be better off implementing
 * hook_synonyms_field_provider_info().
 *
 * @param string $entity_type
 *   Entity type whose synonyms providers are requested
 * @param string $bundle
 *   Bundle name whose synonyms providers are requested
 * @param string $behavior
 *   Behavior name whose synonyms providers are requested
 *
 * @return array
 *   Array of information about synonyms providers your module exposes. Each
 *   sub array will represent a single synonyms provider and should have the
 *   following structure:
 *   - provider: (string) machine name of your synonyms provider. Prefix it with
 *     your module name to make sure no name collisions happen
 *   - label: (string) Human friendly translated name of your synonyms provider
 *   - class: (string) Name of PHP class that implements synonyms behavior
 *     interface, which is stated in synonyms behavior definition. This class
 *     will do all the synonyms providing work. This hook serves pure
 *     declarative function to map entity types, bundles with their synonym
 *     providers
 */
function hook_synonyms_provider_info($entity_type, $bundle, $behavior) {
  $providers = array();

  switch ($entity_type) {
    case 'entity_type_i_want':
      switch ($bundle) {
        case 'bundle_i_want':
          switch ($behavior) {
            case 'behavior_i_want':
              $providers[] = array(
                'provider' => 'my_very_special_synonyms_provider_machine_name',
                'label' => t('This is human friendly name of my synonyms provider. Put something meaningful here'),
                'class' => 'MySynonymsProviderSynonymsBehavior',
              );
              break;
          }
          break;
      }

      break;
  }

  return $providers;
}

/**
 * Example of synonyms provider class.
 *
 * You are encouraged to extend AbstractSynonymsBehavior class as that one
 * contains a few heuristic that make your implementation easier.
 */
class MySynonymsProviderSynonymsBehavior extends AbstractSynonymsBehavior implements AutocompleteSynonymsBehavior {

  /**
   * Extract synonyms from an entity within a specific behavior implementation.
   *
   * @param object $entity
   *   Entity from which to extract synonyms
   *
   * @return array
   *   Array of synonyms extracted from $entity
   */
  public function extractSynonyms($entity) {
    $synonyms = array();

    // Do something with $entity in order to extract synonyms from it. Add all
    // those synonyms into your $synonyms array.

    return $synonyms;
  }

  /**
   * Add an entity as a synonym into another entity.
   *
   * Basically this method should be called when you want to add some entity
   * as a synonym to another entity (for example when you merge one entity
   * into another and besides merging want to add synonym of the merged entity
   * into the trunk entity). You should update $trunk_entity in such a way that
   * it holds $synonym_entity as a synonym (it all depends on how data is stored
   * in your behavior implementation, but probably you will store entity label
   * or its ID as you cannot literaly store an entity inside of another entity).
   * If entity of type $synonym_entity_type cannot be converted into a format
   * expected by your behavior implementation, just do nothing.
   *
   * @param object $trunk_entity
   *   Entity into which another one should be added as synonym
   * @param object $synonym_entity
   *   Fully loaded entity object which has to be added as synonym
   * @param string $synonym_entity_type
   *   Entity type of $synonym_entity
   */
  public function mergeEntityAsSynonym($trunk_entity, $synonym_entity, $synonym_entity_type) {
    // If you can add $synonym_entity into $trunk_entity, then do so.
    // For example:
    $trunk_entity->synonym_storage[] = $synonym_entity;
  }

  /**
   * Look up entities by their synonyms within a behavior implementation.
   *
   * You are provided with a SQL condition that you should apply to the storage
   * of synonyms within the provided behavior implementation. And then return
   * result: what entities match by the provided condition through what
   * synonyms.
   *
   * @param QueryConditionInterface $condition
   *   Condition that defines what to search for. It may contain a placeholder
   *   of AbstractSynonymsSynonymsBehavior::COLUMN_PLACEHOLDER which you should
   *   replace by the column name where the synonyms data for your field is
   *   stored in plain text. For it to do, you may extend the
   *   AbstractSynonymsBehavior class and then just invoke the
   *   AbstractSynonymsBehavior->synonymsFindProcessCondition() method, so you
   *   won't have to worry much about it
   *
   * @return Traversable
   *   Traversable result set of found synonyms and entity IDs to which those
   *   belong. Each element in the result set should be an object and will have
   *   the following structure:
   *   - synonym: (string) Synonym that was found and which satisfies the
   *     provided condition
   *   - entity_id: (int) ID of the entity to which the found synonym belongs
   */
  public function synonymsFind(QueryConditionInterface $condition) {
    // Here, as an example, we'll query an imaginary table where your module
    // supposedly keeps synonyms. We'll also use helpful
    // AbstractSynonymsBehavior::synonymsFindProcessCondition() to normalize
    // $condition argument.
    $query = db_select('my_synonyms_storage_table', 'table');
    $query->addField('table', 'entity_id', 'entity_id');
    $query->addField('table', 'synonym', 'synonym');
    $this->synonymsFindProcessCondition($condition, 'table.synonym');
    $query->condition($condition);
    return $query->execute();
  }
}
