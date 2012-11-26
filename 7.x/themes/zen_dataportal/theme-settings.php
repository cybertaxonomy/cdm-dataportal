<?php
/**
 * Implements hook_form_system_theme_settings_alter().
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 * @param $form_state
 *   A keyed array containing the current state of the form.
 */
function zen_dataportal_form_system_theme_settings_alter(&$form, &$form_state, $form_id = NULL)  {
  // Work-around for a core bug affecting admin themes. See issue #943212.
  if (isset($form_id)) {
    return;
  }

  // Create the form using Forms API: http://api.drupal.org/api/7

  $which_image = "banner";
  $form['zen_dataportal'] = array(
      '#type' => 'fieldset',
      '#title' => t($which_image) . ' ' . t('image settings'),
      '#description' => t('If toggled on, the following image will be displayed.'),
      '#attributes' => array('class' => array('theme-settings-bottom')),
    );
    $form['zen_dataportal']['default_' . $which_image] = array(
      '#type' => 'checkbox',
      '#title' => t('Use the default') . t($which_image),
      '#default_value' => theme_get_setting('default_' . $which_image),
      '#tree' => FALSE,
      '#description' => t('Check here if you want the theme to use the image supplied with it.')
    );
    $form['zen_dataportal']['settings'] = array(
      '#type' => 'container',
      '#states' => array(
        // Hide the logo settings when using the default logo.
        'invisible' => array(
          'input[name="default_' . $which_image . '"]' => array('checked' => TRUE),
        ),
      ),
    );

    // If $path is a public:// URI, display the path relative to the files
    // directory; stream wrappers are not end-user friendly.
    $path = theme_get_setting($which_image . '_path');
    $image = '';
    if (file_uri_scheme($path) == 'public') {
      $url = file_create_url($path);
      $path = file_uri_target($path);
      $form['zen_dataportal']['settings'][$which_image . '_preview'] = array(
      	'#type' => 'item',
        '#title' => t('Preview'),
        '#markup' => '<div class="image-preview"><img src="' . $url . '"/></div>',
      );
    }
    $form['zen_dataportal']['settings'][$which_image . '_path'] = array(
      '#type' => 'textfield',
      '#title' => t('Path to custom') . ' ' . t($which_image),
      '#description' => t('The path to the file you would like to use as your') . ' ' . t($which_image),
      '#default_value' => $path,
    );
    $form['zen_dataportal']['settings'][$which_image . '_upload'] = array(
      '#type' => 'file',
      '#title' => t('Upload') . ' ' . t($which_image),
      '#maxlength' => 40,
      '#description' => t("If you don't have direct file access to the server, use this field to upload your image.")
    );


  $form['#validate'][] = 'zen_dataportal_theme_settings_validate';
  $form['#submit'][] = 'zen_dataportal_theme_settings_submit';


  // We are editing the $form in place, so we don't need to return anything.
}

/**
 * Process zen_dataportal admin form submissions.
 */
function zen_dataportal_theme_settings_submit($form, &$form_state) {

  $values = &$form_state['values'];

  // If the user uploaded a new image, save it to a permanent location
  // and use it in place of the default theme-provided file.
  $which_image = "banner";
  if ($file = $values[$which_image . '_upload']) {
    unset($values[$which_image . '_upload']);
    $filename = file_unmanaged_copy($file->uri);
    $values['default_' .$which_image ] = 0;
    $values[$which_image . '_path'] = $filename;
    $values['toggle_' . $which_image] = 1;
  }
  if (!empty($values[$which_image . '_path'])) {
    $values[$which_image . '_path'] = _system_theme_settings_validate_path($values[$which_image . '_path']);
  }

  /*
   * Ok, we are done here the $values will be saved in the theme
   * variable by system_theme_settings_submit($form, &$form_state)
   */

}

/**
 * Validator for the system_theme_settings() form.
 */
function zen_dataportal_theme_settings_validate($form, &$form_state) {

  $validators = array('file_validate_is_image' => array());

  $which_image = "banner";
  // Check for a new uploaded logo.
  $file = file_save_upload($which_image . '_upload', $validators);
  if (isset($file)) {
    // File upload was attempted.
    if ($file) {
      // Put the temporary file in form_values so we can save it on submit.
      $form_state['values'][$which_image . '_upload'] = $file;
    }
    else {
      // File upload failed.
      form_set_error($which_image . '_upload', t('The image could not be uploaded.'));
    }
  }
}