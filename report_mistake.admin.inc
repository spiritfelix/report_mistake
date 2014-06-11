<?php
/**
 * @file
 * For Admin pages.
 */

/**
 * Implements hook_form().
 */
function report_mistake_types_admin_form($form, &$form_state) {
  if (user_access('admin report mistake')) {
    if ($cache = cache_get('report_mistake_types_options')) {
      $content_types_options = unserialize($cache->data);
    }
    else {
      $types = node_type_get_types();
      $content_types_options = array();
      foreach ($types as $key => $value) {
        $content_types_options[$key] = $value->name;
      }
      cache_set('report_mistake_types_options', serialize($content_types_options));
    }
    if ($cache = cache_get('report_mistake_field_types_options')) {
      $field_types_options = unserialize($cache->data);
    }
    else {
      $types = field_info_field_types();
      $field_types_options = array();
      foreach ($types as $key => $value) {
        $field_types_options[$key] = $value['label'];
      }
      cache_set('report_mistake_field_types_options', serialize($field_types_options));
    }

    $form['report_mistake_content_types'] = array(
      '#type' => 'fieldset' ,
      '#collapsible' => TRUE ,
      '#title' => t('Content Types'),
    );
    $form['report_mistake_content_types']['report_mistake_content_types'] = array(
      '#type' => 'checkboxes',
      '#description' => t('Select content types to use "Report mistake" module.'),
      '#options' => $content_types_options,
      '#default_value' => variable_get('report_mistake_content_types', array('')),
    );
    $form['report_mistake_field_types'] = array(
      '#type' => 'fieldset' ,
      '#collapsible' => TRUE ,
      '#title' => t('Field Types'),
    );
    $form['report_mistake_field_types']['report_mistake_field_types'] = array(
      '#type' => 'checkboxes',
      '#description' => t('Select field types to use "Report mistake" module.'),
      '#options' => $field_types_options,
      '#default_value' => variable_get('report_mistake_field_types', array('')),
    );

    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );
    return $form;
  }
}

/**
 * List of un-operated reports, with operations.
 */
function report_mistake_admin_list() {
  if (user_access('admin report mistake')) {
    drupal_add_library('system', 'drupal.ajax');

    $conditions = array(array("status", "0", "="));
    $result = report_mistake_get_list($conditions);
    $header = array(
      t('id'),
      t('Content'),
      t('Report user'),
      t('Report field'),
      t('Delta'),
      t('value'),
      t('Reference link'),
      t('Date'),
      '',
      '',
      '',
    );
    $rows = array();
    foreach ($result as $item) {
      $rid = $item->report_id;
      $node = node_load($item->nid);
      $title = l($node->title, 'node/' . $item->nid);
      $node_type = $node->type;
      $info = field_info_instance('node', $item->field_name, $node_type);
      $field_label = $info['label'];
      $account = user_load($item->uid);
      $user_name = l($account->name, 'user/' . $item->uid);
      $link = l($item->link, $item->link);
      $created = format_date($item->created);

      $row = array(
        'report_id' => $rid,
        'node' => $title,
        'user' => $user_name,
        'field_name' => $field_label,
        'delta' => $item->delta,
        'value' => $item->value,
        'link' => $link,
        'created' => $created,
      );
      $row[] = l(t('View'), 'node/' . $item->nid);
      $row[] = l(t('Accept'), 'report_mistake/admin/ajax/' . $rid . '/accept', array('attributes' => array('class' => 'use-ajax')));
      $row[] = l(t('Ignore'), 'report_mistake/admin/ajax/' . $rid . '/ignore', array('attributes' => array('class' => 'use-ajax')));
      $row_class = 'report-' . $rid;
      $rows[] = array('data' => $row, 'class' => array($row_class));
    }
    return theme(
      'table',
      array(
        'header' => $header,
        'rows' => $rows,
        'attributes' => array(
          'class' => array('report-mistake-table'),
        ),
      )
    );
  }
}

/**
 * List of operated reports.
 */
function report_mistake_admin_history() {
  if (user_access('admin report mistake')) {
    $conditions = array(array("status", "0", ">"));
    $result = report_mistake_get_list($conditions);
    $header = array(
      t('id'),
      t('Content'),
      t('Report user'),
      t('Report field'),
      t('Delta'),
      t('value'),
      t('Reference link'),
      t('Date'),
      t('Operator'),
      t('Status'),
    );
    $rows = array();
    $status_array = array('0' => 'New', '1' => 'Fixed', '2' => 'Ignored');
    foreach ($result as $item) {
      $rid = $item->report_id;
      $node = node_load($item->nid);
      $title = l($node->title, 'node/' . $item->nid);
      $node_type = $node->type;
      $info = field_info_instance('node', $item->field_name, $node_type);
      $field_label = $info['label'];
      $account = user_load($item->uid);
      $user_name = l($account->name, 'user/' . $item->uid);
      $link = l($item->link, $item->link);
      $created = format_date($item->created);
      $operator_user = user_load($item->op_uid);
      $operator = l($operator_user->name, 'user/' . $item->op_uid);
      $status = $status_array[$item->status];
      $row = array(
        'report_id' => $rid,
        'node' => $title,
        'user' => $user_name,
        'field_name' => $field_label,
        'delta' => $item->delta,
        'value' => $item->value,
        'link' => $link,
        'created' => $created,
        'operator' => $operator,
        'status' => $status,
      );
      $rows[] = $row;
    }
    return theme('table', array('header' => $header, 'rows' => $rows));
  }
}

/**
 * Implements hook_form_submit().
 */
function report_mistake_types_admin_form_submit($form, $form_state) {
  $report_mistake_content_types = $form_state['values']['report_mistake_content_types'];
  $report_mistake_field_types = $form_state['values']['report_mistake_field_types'];
  variable_set('report_mistake_content_types', $report_mistake_content_types);
  variable_set('report_mistake_field_types', $report_mistake_field_types);
  drupal_set_message(t('The settings have been saved.'));
  cache_clear_all();
}

/**
 * Ajax admin proprocess.
 */
function report_mistake_ajax_admin($ajax, $rid, $op) {
  global $user;
  $status_map = array('accept' => 1, 'ignore' => 2);
  $op_uid = $user->uid;
  $changed = FALSE;

  if ($op == 'accept' && $report = report_mistake_get_report($rid)) {
    $nid = $report['nid'];
    $field_name = $report['field_name'];
    $delta = $report['delta'];
    $value = $report['value'];
    $node = node_load($nid);
    try {
      $node_wrapper = entity_metadata_wrapper('node', $node);
      $node_wrapper->$field_name->offsetSet($delta, $value);
      $node_wrapper->save();
      $changed = TRUE;
    }
    catch (EntityMetadataWrapperException $exc) {
      watchdog(
        'Report Mistake',
        'See '  . __FUNCTION__ . '() ' .  $exc->getTraceAsString(),
         NULL, WATCHDOG_ERROR
      );
      $changed = FALSE;
    }
  }
  if ($changed || $op == 'ignore') {
    $query = db_update('report_mistake');
    $query->fields(array('status' => $status_map[$op], 'op_uid' => $op_uid));
    $query->condition('report_id', $rid);
    $query->execute();
    if ($ajax) {
      $commands = array();
      $commands[] = ajax_command_remove('tr.report-' . $rid);
      $commands[] = ajax_command_restripe("table.report-mistake-table");
      print ajax_render($commands);
      drupal_exit();
    }
    else {
      $message = t('Deleted 1 message');
      drupal_set_message($message);
      drupal_goto();
    }
  }
}

/**
 * Get report list by conditions.
 */
function report_mistake_get_list($conditions = array()) {
  $query = db_select('report_mistake', 'rm');
  $query->fields('rm');
  if (isset($conditions)) {
    foreach ($conditions as $condition) {
      $query->condition($condition[0], $condition[1], $condition[2]);
    }
  }
  $query->orderBy('created', 'DESC');
  $result = $query->execute()->fetchAll();
  return $result;
}

/**
 * Get report.
 */
function report_mistake_get_report($rid) {
  if (is_numeric($rid)) {
    $query = db_select('report_mistake', 'rm');
    $query->fields('rm');
    $query->condition('report_id', $rid, '=');
    $result = $query->execute()->fetchAssoc();
    return $result;
  }
  else {
    return FALSE;
  }
}
