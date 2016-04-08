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

  /**
   * Extract synonyms from an entity within a specific behavior implementation.
   *
   * @param object $entity
   *   Entity from which to extract synonyms
   *
   * @return array
   *   Array of synonyms extracted from $entity
   */
  public function extractSynonyms($entity);

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
  public function mergeEntityAsSynonym($trunk_entity, $synonym_entity, $synonym_entity_type);

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
  public function synonymsFind(QueryConditionInterface $condition);
}

/**
 * Exception thrown by implementations of SynonymsBehavior interface.
 */
class SynonymsBehaviorException extends Exception {}

/**
 * Starting point for implementing SynonymsBehavior interface.
 */
abstract class AbstractSynonymsBehavior implements SynonymsBehavior {

  /**
   * Constant which denotes placeholder of a synonym column.
   *
   * @var string
   */
  const COLUMN_PLACEHOLDER = '***COLUMN***';

  /**
   * Behavior implementation on which this class was initialized.
   *
   * @var array
   */
  protected $behavior_implementation;

  public function __construct($behavior_implementation) {
    $this->behavior_implementation = $behavior_implementation;
  }

  /**
   * Process condition in 'synonymsFind' method.
   *
   * Process condition in 'synonymsFind' method replacing all references of
   * synonym column with the real name of that column.
   *
   * @param QueryConditionInterface $condition
   *   Condition that should be processed
   * @param string $column
   *   Real name of the synonym column
   */
  protected function synonymsFindProcessCondition(QueryConditionInterface $condition, $column) {
    $condition_array = &$condition->conditions();
    foreach ($condition_array as &$v) {
      if (is_array($v) && isset($v['field'])) {
        if ($v['field'] instanceof QueryConditionInterface) {
          // Recursively process this condition too.
          $this->synonymsFindProcessCondition($v['field'], $column);
        }
        else {
          $v['field'] = str_replace(self::COLUMN_PLACEHOLDER, $column, $v['field']);
        }
      }
    }
  }
}

/**
 * Interface of the autocomplete synonyms behavior.
 */
interface AutocompleteSynonymsBehavior extends SynonymsBehavior {
}

/**
 * Interface of the synonyms friendly select behavior.
 */
interface SelectSynonymsBehavior extends SynonymsBehavior {
}
