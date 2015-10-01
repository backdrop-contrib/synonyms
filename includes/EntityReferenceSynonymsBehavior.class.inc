<?php

/**
 * @file
 * Enables Entity Reference field type to be source of synonyms.
 */

/**
 * Definition of EntityReferenceSynonymsBehavior class.
 */
class EntityReferenceSynonymsBehavior implements SynonymsSynonymsBehavior, AutocompleteSynonymsBehavior, SearchSynonymsBehavior {

  public function extractSynonyms($items, $field, $instance, $entity, $entity_type) {
    $synonyms = array();

    $target_tids = array();
    foreach ($items as $item) {
      $target_tids[] = $item['target_id'];
    }
    $entities = entity_load($field['settings']['target_type'], $target_tids);
    foreach ($entities as $entity) {
      $synonyms[] = entity_label($field['settings']['target_type'], $entity);
    }

    return $synonyms;
  }

  public function processEntityFieldQuery($tag, EntityFieldQuery $query, $field, $instance) {
    // Unfortunately EntityFieldQuery does not currently support INNER JOINing
    // referenced entities via any field type.
    // Thus, we use an ugly solution -- going through all entities that exist
    // in such entity type trying to see if there is a match by entity's label.
    $efq = $this->targetEntityFieldQuery($field, $instance);

    $label_column = self::getLabelColumn($field, $instance);
    if (!$label_column) {
      // We can't get any matches if we do not know what column to query
      // against. So we add a condition to $query which will 100% yield empty
      // results.
      return FALSE;
    }
    $efq->propertyCondition($label_column, '%' . $tag . '%', 'LIKE');
    $result = $efq->execute();

    if (!isset($result[$field['settings']['target_type']]) || !is_array($result[$field['settings']['target_type']])) {
      return FALSE;
    }
    $result = $result[$field['settings']['target_type']];
    $query->fieldCondition($field, 'target_id', array_keys($result));

    return TRUE;
  }

  public function mergeEntityAsSynonym($items, $field, $instance, $synonym_entity, $synonym_entity_type) {
    // Firstly validating that this entity reference is able to reference to
    // that type of entity.
    $expected_synonym_entity_type = $field['settings']['target_type'];
    if ($expected_synonym_entity_type != $synonym_entity_type) {
      return array();
    }
    $synonym_entity_id = entity_id($synonym_entity_type, $synonym_entity);
    return array(array(
      'target_id' => $synonym_entity_id,
    ));
  }

  public function synonymFind($synonym, $field, $instance, $entity_type, $bundle = NULL) {
    $efq = $this->targetEntityFieldQuery($field, $instance);
    $label_column = $this->getLabelColumn($field, $instance);
    if ($label_column) {
      $efq->propertyCondition($label_column, $synonym);
      $result = $efq->execute();
      if (isset($result[$field['settings']['target_type']])) {
        $entity_ids = array_keys($result[$field['settings']['target_type']]);
        $efq = new EntityFieldQuery();
        $efq->entityCondition('entity_type', $entity_type);
        if ($bundle) {
          $efq->entityCondition('bundle', $bundle);
        }
        $efq->fieldCondition($field, 'target_id', $entity_ids);
        $result = $efq->execute();
        if (isset($result[$entity_type])) {
          return array_keys($result[$entity_type]);
        }
      }
    }
    return array();
  }

  /**
   * Supportive method to query the referenced entities through provided field.
   *
   * Prepare an EntityFieldQuery to query the target entities of the provided
   * entity reference field.
   *
   * @param array $field
   *   Field API field definition array of the entity reference field, based on
   *   which the EntityFieldQuery object should be built
   * @param array $instance
   *   Field API instance definition array of the entity reference field, based
   *   on which the EntityFieldQuery object should be built
   *
   * @return EntityFieldQuery
   */
  protected function targetEntityFieldQuery($field, $instance) {
    $efq = new EntityFieldQuery();
    $efq->entityCondition('entity_type', $field['settings']['target_type']);
    return $efq;
  }

  /**
   * Find out label column of the referenced entity type.
   *
   * Supportive method to find out in what column of the referenced entity type
   * the entity label is stored.
   *
   * @param array $field
   *   Field API field definition array of the entity reference field, whose
   *   label column should be determined
   * @param array $instance
   *   Field API instance definition array of the entity reference field, whose
   *   label column should be determined
   *
   * @return string|bool
   *   Label column of the referenced entity type. If it cannot be determined,
   *   FALSE is returned
   */
  protected function getLabelColumn($field, $instance) {
    $entity_info = entity_get_info($field['settings']['target_type']);
    return isset($entity_info['entity keys']['label']) ? $entity_info['entity keys']['label'] : FALSE;
  }
}
