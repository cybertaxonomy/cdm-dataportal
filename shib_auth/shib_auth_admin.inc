<?php
// $Id: shib_auth_admin.inc,v 1.1.2.5 2008/09/26 11:58:24 niif Exp $

/**
 * @file
 * Generate the administration form of the Shibboleth authentication module
 *
 * @returns HTML text of the administration form
 */
function shib_auth_admin() {
  $form = array();

  $form['shib_handler_settings'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Shibboleth handler settings'),
    '#weight'      => 0,
    '#collapsible' => FALSE,
  );

  $form['shib_attribute_settings'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Attribute settings'),
    '#weight'      => 0,
    '#collapsible' => FALSE,
  );

  $form['shib_handler_settings']['shib_auth_handler_url'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Shibboleth handler URL'),
    '#default_value' => variable_get('shib_auth_handler_url', '/Shibboleth.sso'),
    '#description'   => t('The URL can be absolute or relative to the server base url: http://www.example.com/Shibboleth.sso; /Shibboleth.sso'),
  );

  $form['shib_handler_settings']['shib_auth_handler_protocol'] = array(
    '#type'          => 'select',
    '#title'         => t('Shibboleth handler protocol'),
    '#default_value' => variable_get('shib_auth_handler_protocol', 'https'),
    '#options'       => array(
      'http'  => t('HTTP'),
      'https' => t('HTTPS'),
    ),
    '#description'   => t('This option will be effective only if the handler URL is a relative path.'),
  );

  $form['shib_handler_settings']['shib_auth_wayf_uri'] = array(
    '#type'          => 'textfield',
    '#title'         => t('WAYF location'),
    '#default_value' => variable_get('shib_auth_wayf_uri', '/WAYF/HREF'),
  );
  
  $form['shib_handler_settings']['login_target_protocol'] = array(
    '#type'          => 'select',
    '#title'         => t('Shibboleth login target protocol'),
    '#default_value' => variable_get('login_target_protocol', 'https'),
    '#options'       => array(
      'http'  => t('HTTP'),
      'https' => t('HTTPS'),
    ),
    '#description'   => t('The protocol to use as target protocol after successful login.'),
  );

  global $base_url;
  $form['shib_handler_settings']['logout_return_url'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Shibboleth logout return URL'),
    '#default_value' => variable_get('logout_return_url', $base_url),
    '#description'   => t('The URL to return to after logout. Must be an absolute URL.'),
  );

  $form['shib_attribute_settings']['shib_auth_username_variable'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Server variable for username'),
    '#default_value' => variable_get('shib_auth_username_variable', 'REMOTE_USER'),
  );

  $form['shib_attribute_settings']['shib_auth_username_email'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Server variable for e-mail address'),
    '#default_value' => variable_get('shib_auth_username_email', 'HTTP_SHIB_MAIL'),
  );

  $form['shib_attribute_debug']['shib_auth_debug_state'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Enable DEBUG mode.'),
    '#default_value' => variable_get('shib_auth_debug_state', FALSE),
  );
  
  return system_settings_form($form);
} // function shib_auth_admin()

/**
 * Clone an exists rule form based on new rule add form
 */
function shib_auth_clone_rule() {
  return shib_auth_edit_rule();
}//function shib_auth_clone_rule()

/**
 * Submit the cloned rule based on new rule add submit
 */
function shib_auth_clone_rule_submit($form_id, $form_values) {
  shib_auth_new_rule_form_submit($form_id, $form_values); 
}

/**
 * Delete a rule from the list.
 *
 * @param $rule_id Id of the rule what will be delete.
 */
function shib_auth_delete_rule($rule_id) {
  $result = db_query("DELETE FROM {shib_auth} WHERE id = %d", $rule_id);
  
  if ($result) {
    drupal_set_message('Rule <span style="font-weight: bold;">#'. $rule_id .'</span> has been deleted.', 'warning');
  }
  else {
    $message = 'Rule has not been deleted!';
    drupal_set_message(t($message), 'error');
    watchdog('shub_auth', $message, WATCHDOG_ERROR);
  }
  
  drupal_goto('admin/user/shib_auth/rules');
}//function shib_auth_delete_rule()

/**
 * List the added rule(s)
 *
 * @return Rule list in a HTML table
 */
function shib_auth_list_rules() {
  $header = array(
              array('data' => t('Attribute'), 'field' => 'field'),
              array('data' => t('RegExp'), 'field' => 'regexpression'),
              array('data' => t('Role(s)'), 'field' => 'role'),
              array('data' => t('Actions')),
            );
  $tablesort_sql = 'SELECT * FROM {shib_auth}'. tablesort_sql($header);
  $rules = db_query($tablesort_sql);

  if (!$rules) {
    return t('There is no rule in the database');
  }
  
  $row = array();
  while ($rule = db_fetch_array($rules)) {
    $user_roles = user_roles(TRUE);
    $roles = unserialize($rule['role']);
    $roles_list = '';
    if (!empty($roles)) $roles_list = implode(', ', $roles);
    
    $links = l(t('Clone') .' ', 'admin/user/shib_auth/rules/Clone/'. $rule['id']);
    $links .= l(t('Edit') .' ', 'admin/user/shib_auth/rules/Edit/'. $rule['id']);
    $links .= l(t('Delete') .' ', 'admin/user/shib_auth/rules/Delete/'. $rule['id']);
    
    $row[] = array($rule['field'], $rule['regexpression'], $roles_list, $links);
  }
  
  return theme_table($header, $row);
} // function shib_auth_list_rules()

/**
 * Rule edit form basedon new rule form
 *
 * @return array rule edit form array
 */
function shib_auth_edit_rule() {
  $rule_id = arg(5);
  
  $rule = db_query("SELECT * FROM {shib_auth} WHERE id = %d", array($rule_id));
  $exists_rule = db_fetch_array($rule);

  return shib_auth_new_rule_form($exists_rule);
}//function shib_auth_edit_rule()

/**
 * Submit the edited rule.
 * 
 * @param $form_id
 *  Id of the form.
 * @param $form_values
 *   Values of the POSTed form.
 */
function shib_auth_edit_rule_submit($form_id, $form_values) {
  $roles = _role_normalize($form_values['roles']);

  $result = db_query("UPDATE {shib_auth} SET field = '%s', regexpression = '%s', role = '%s' WHERE id = %d", $form_values['field'], $form_values['regexpression'], serialize($roles), arg(5));
  
  if ($result) {
    drupal_set_message(t('Rule was save.'));
    drupal_goto('admin/user/shib_auth/rules');
  }
  else {
    $message = 'Rule was not save into database!';
    drupal_set_message(t($message));
    watchdog('shib_auth_error', $message, WATCHDOG_ERROR);
  }

}

/**
 * Add a new rule form.
 * 
 * @param $editing_rule
 *  If edit or clone a rule it has value otherwise it is an empty array.
 * @return
 *  form API compatible array
 */
function shib_auth_new_rule_form($editing_rule = array()) {
  $form = array();
  
  $form['field'] = array(
    '#title'          => t('Shibboleth attribute name'),
    '#type'           => 'textfield',
    '#default_value'  => !empty($editing_rule['field'])?$editing_rule['field']:'',
    '#require'        => TRUE,
    '#description'    => t('More properly: <b>$_SERVER</b> field name; enable DEBUG mode to list available fields. <br/>Note that it might differ from your users\' fields.'),
  );
  
  $form['regexpression'] = array(
    '#title'          => t('Value (regexp)'),
    '#type'           => 'textfield',
    '#default_value'  => !empty($editing_rule['regexpression'])?$editing_rule['regexpression']:'',
    '#require'        => TRUE,
  );

  $roles = user_roles(TRUE);
  
  $edit_role = unserialize($editing_rule['role']);
  $form['roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles'),
    '#default_value' => is_array($edit_role)?array_keys($edit_role):'',
    '#options' => $roles,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('@Action rule', array('@Action' => arg(4))),
  );

  //$form['#submit'][] = 'shib_auth_new_rule';
  
  return $form;
}//function shib_auth_new_rule_form()

/** 
 * New rule add submit implementation
 * 
 * @param $form_id 
 *  Id of the form what you process.
 * @param $form_values 
 *  Form values.
 */
function shib_auth_new_rule_form_submit($form_id, $form_values) {
  $roles = _role_normalize($form_values['roles']);

  $result = db_query("INSERT INTO {shib_auth} ".
                     "VALUES (%d, '%s', '%s', '%s' )", 
                    db_next_id('{shib_auth}_id'), 
                    $form_values['field'], 
                    $form_values['regexpression'], 
                    serialize($roles));
  if ($result) {
    drupal_set_message(t('Rule was save.'));
    drupal_goto('admin/user/shib_auth/rules');
  }
  else {
    $message = 'Rule was not save into database!';
    drupal_set_message(t($message));
    watchdog('shib_auth_error', $message, WATCHDOG_ERROR);
  }
}// function shib_auth_new_rule_form_submit


function _role_normalize($role_id) {
  $user_role = user_roles(TRUE);
  $roles = array();
 
  foreach ($role_id AS $key => $role_key) {
    if ($role_key !== 0) {
      $roles[$role_key] = $user_role[$role_key];
    }
  }

  return $roles;
}
