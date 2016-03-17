<?php

/**
 * @file
 * Enables Commerce Product Reference field type for synonyms integration.
 */

/**
 * Definition of CommerceProductReferenceSynonymsBehavior class.
 */
class CommerceProductReferenceSynonymsBehavior extends AbstractSynonymsBehavior implements SynonymsBehavior {

  public function extractSynonyms($items, $field, $instance, $entity, $entity_type) {
    $synonyms = array();

    $product_ids = array();
    foreach ($items as $item) {
      $product_ids[] = $item['product_id'];
    }
    $entities = commerce_product_load_multiple($product_ids);
    foreach ($entities as $entity) {
      $synonyms[] = entity_label('commerce_product', $entity);
    }

    return $synonyms;
  }

  public function mergeEntityAsSynonym($items, $field, $instance, $synonym_entity, $synonym_entity_type) {
    if ('commerce_product' != $synonym_entity_type) {
      return array();
    }
    $synonym_entity_id = entity_extract_ids($synonym_entity_type, $synonym_entity);
    return array(array(
      'product_id' => $synonym_entity_id[0],
    ));
  }

  public function synonymItemHash($item, $field, $instance) {
    return $item['product_id'];
  }

  public function synonymsFind(QueryConditionInterface $condition, $field, $instance) {
    if ($field['storage']['type'] != 'field_sql_storage') {
      throw new SynonymsSynonymsBehaviorException(t('Not supported storage engine %type in synonymsFind() method.', array(
        '%type' => $field['storage']['type'],
      )));
    }
    $table = array_keys($field['storage']['details']['sql'][FIELD_LOAD_CURRENT]);
    $table = reset($table);
    $column = $field['storage']['details']['sql'][FIELD_LOAD_CURRENT][$table]['product_id'];

    $query = db_select($table, 'field');

    $product_entity_type_info = entity_get_info('commerce_product');

    $product_entity_alias = $query->innerJoin($product_entity_type_info['base table'], 'product', 'field.' . $column . ' = %alias.' . $product_entity_type_info['entity keys']['id']);
    $query->addField($product_entity_alias, $product_entity_type_info['entity keys']['label'], 'synonym');
    $query->fields('field', array('entity_id'));
    $query->condition('field.entity_type', $instance['entity_type']);
    $query->condition('field.bundle', $instance['bundle']);

    $this->synonymsFindProcessCondition($condition, $product_entity_alias . '.' . $product_entity_type_info['entity keys']['label']);
    $query->condition($condition);
    return $query->execute();
  }
}
