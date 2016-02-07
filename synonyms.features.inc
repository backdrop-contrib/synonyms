<?php

/**
 * @file
 * Synonyms module integration with Features.
 */

/**
 * Implements hook_features_export().
 */
function synonyms_features_export($data, &$export, $module_name) {
  $pipe = array();

  $export['features']['synonyms'] = array();
  foreach ($data as $v) {
    list($entity_type, $bundle, $field_name, $behavior) = explode(':', $v);
    $behavior_definition = synonyms_behaviors();
    $behavior_definition = $behavior_definition[$behavior];
    $export['dependencies'][] = $behavior_definition['module'];

    $export['features']['synonyms'][$v] = $v;

    $pipe['field'][] = implode('-', array($entity_type, $bundle, $field_name));
  }

  $export['dependencies'][] = 'synonyms';
  $export['dependencies'] = array_unique($export['dependencies']);
  return $pipe;
}

/**
 * Implements hook_features_export_options().
 */
function synonyms_features_export_options() {
  $options = array();

  foreach (synonyms_behaviors() as $behavior => $behavior_definition) {
    foreach (synonyms_behavior_get($behavior, NULL, NULL, TRUE) as $behavior_implementation) {
      $instance = synonyms_instance_id_load($behavior_implementation['instance_id']);

      $key = array(
        $instance['entity_type'],
        $instance['bundle'],
        $instance['field_name'],
        $behavior_implementation['behavior'],
      );
      $options[implode(':', $key)] = implode('-', $key);
    }
  }

  return $options;
}

/**
 * Implements hook_features_export_render().
 */
function synonyms_features_export_render($module_name, $data, $export = NULL) {
  $code = array();
  $code[] = '  $synonyms = array();';
  foreach ($data as $name) {
    list($entity_type, $bundle, $field_name, $behavior) = explode(':', $name);
    $behavior_implementation = synonyms_behavior_get($behavior, $entity_type, $bundle, TRUE);
    foreach ($behavior_implementation as $v) {
      if ($v['field_name'] == $field_name) {
        $behavior_implementation = $v;
        break;
      }
    }
    unset($behavior_implementation['instance_id'], $behavior_implementation['settings_serialized']);
    $code[] = "  \$synonyms['{$name}'] = " . features_var_export($behavior_implementation, '  ') .";";
  }
  $code[] = "  return \$synonyms;";
  $code = implode("\n", $code);
  return array('default_synonyms' => $code);
}

/**
 * Implements hook_features_revert().
 */
function synonyms_features_revert($module_name) {
  $synonyms = module_invoke($module_name, 'default_synonyms');
  foreach ($synonyms as $v) {
    $instance = field_info_instance($v['entity_type'], $v['field_name'], $v['bundle']);
    if ($instance) {
      synonyms_behavior_settings_save(array(
        'instance_id' => $instance['id'],
        'behavior' => $v['behavior'],
        'settings' => $v['settings'],
      ));
    }
  }
}

/**
 * Implements hook_features_rebuild().
 */
function synonyms_features_rebuild($module_name) {
  $synonyms = module_invoke($module_name, 'default_synonyms');
  foreach ($synonyms as $v) {
    $instance = field_info_instance($v['entity_type'], $v['field_name'], $v['bundle']);
    if ($instance) {
      synonyms_behavior_settings_save(array(
        'instance_id' => $instance['id'],
        'behavior' => $v['behavior'],
        'settings' => $v['settings'],
      ));
    }
  }
}
