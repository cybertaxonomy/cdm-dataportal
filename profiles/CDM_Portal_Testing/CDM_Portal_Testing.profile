<?php
/**
 * @file
 * Enables modules and site configuration for a CDM portal installation for testing.
 */

/**
 * Implements hook_form_FORM_ID_alter() for install_configure_form().
 *
 * Allows the profile to alter the site configuration form.
 */
function CDM_Portal_Testing_form_install_configure_form_alter(&$form, $form_state) {
  // Pre-populate the form.
  $form['site_information']['site_name']['#default_value'] = st('EDIT CDM DataPortal');
  $form['site_information']['site_mail']['#default_value'] = 'admin@edit.cdm.dataportal.eu';
  $form['admin_account']['account']['name']['#default_value'] = 'admin';
  $form['admin_account']['account']['mail']['#default_value'] = 'admin@edit.cdm.dataportal.eu';

  // @Comment WA: enable if default password is required (drupal password_confirm field does not allow default value).
  // $form['admin_account']['account']['pass']['#type'] = 'textfield';
  // $form['admin_account']['account']['pass']['#title'] = st('Password');
  // $form['admin_account']['account']['pass']['#default_value'] = 'admin';
  $form['server_settings']['site_default_country']['#default_value'] = 'DE';
}
