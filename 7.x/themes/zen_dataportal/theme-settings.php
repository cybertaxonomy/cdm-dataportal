<?php

/**
 * Provides the image names which can be configured in the theme settings to all
 * theme setting functions.
 *
 * @return multitype:string
 *     the list of image names which can be configured in the theme settings
 */
function _zen_dataportal_imagenames() {
  static $image_list = array('banner', 'body_background', 'page_background');
  return $image_list;
}

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

  $form['zen_dataportal_colors'] = array(
          '#type' => 'fieldset',
          '#title' => t('Colors'),
          '#description' => t('Configure colors where.'),
          '#attributes' => array('class' => array('theme-settings-bottom')),
  );
  $form['zen_dataportal_colors']['site_name_color'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Site name color'),
      '#default_value' => theme_get_setting('site_name_color'),
      '#description'   => t('Set the color of the site name which is shown in the header. Must be a css color value like: #000000'),
  );

  //
  // custom images for banner, body and page backgrounds
  //
  foreach(_zen_dataportal_imagenames() as $which_image) {
    zen_dataportal_form_widget_image($form, $which_image);
  }


  $form['#validate'][] = 'zen_dataportal_theme_settings_validate';
  $form['#submit'][] = 'zen_dataportal_theme_settings_submit';
  // We are editing the $form in place, so we don't need to return anything.
}

/**
 * Process zen_dataportal admin form submissions.
 * @param $form
 *   Nested array of form elements that comprise the form.
 * @param $form_state
 *   A keyed array containing the current state of the form.
 */
function zen_dataportal_theme_settings_submit($form, &$form_state) {


  foreach(_zen_dataportal_imagenames() as $which_image) {
    zen_dataportal_form_widget_image_submit($form_state, $which_image);
  }

  // update logo information like wich one to use and size, this is needed to
  // offset the banner and menu bar
  $values = &$form_state['values'];
  if(!theme_get_setting('default_logo')) {
    $logo_file = path_to_theme() . '/logo.png';
  } else {
    $logo_file = theme_get_setting('logo_path');
  }
  $logo_file_info = image_get_info($logo_file);
  if(isset($logo_file_info['width'])) {
   $values['logo_size'] = array('width' => $logo_file_info['width'], 'height' => $logo_file_info['height']);
  }



  /*
   * Ok, we are done here the $values will be saved in the theme
   * variable by system_theme_settings_submit($form, &$form_state)
   * in modules/system/system.admin.inc
   */
}

/**
 * Validator for the system_theme_settings() form.
 *  @param $form
 *   Nested array of form elements that comprise the form.
 * @param $form_state
 *   A keyed array containing the current state of the form.
 */
function zen_dataportal_theme_settings_validate($form, &$form_state) {

  foreach(_zen_dataportal_imagenames() as $which_image) {
    zen_dataportal_form_widget_image_validate($form_state, $which_image);
  }
}

/*******************************************************************
 *                    form widget functions
 *******************************************************************/

/**
 *
 * Enter description here ...
 *  @param $form
 *   Nested array of form elements that comprise the form.
 * @param unknown_type $which_image
 */
function zen_dataportal_form_widget_image(&$form, $which_image){
  $image_label = str_replace('_', ' ', $which_image);
  $form['zen_dataportal_' .  $which_image] = array(
          '#type' => 'fieldset',
          '#title' => ucfirst(t($image_label)) . ' ' . t('image settings'),
          '#description' => t('If toggled on, the following image will be displayed.'),
          '#attributes' => array('class' => array('theme-settings-bottom')),
  );
  $form['zen_dataportal_' .  $which_image]['default_' . $which_image] = array(
          '#type' => 'checkbox',
          '#title' => t('Use the default') . t($which_image),
          '#default_value' => theme_get_setting('default_' . $which_image),
          '#tree' => FALSE,
          '#description' => t('Check here if you want the theme to use the image supplied with it.')
  );
  $form['zen_dataportal_' .  $which_image]['settings'] = array(
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
    $form['zen_dataportal_' .  $which_image]['settings'][$which_image . '_preview'] = array(
          	'#type' => 'item',
            '#title' => t('Preview'),
            '#markup' => '<div class="image-preview"><img src="' . $url . '"/></div>',
    );
  } // FIXME manully entered files without scheme public: are omitted here
  $form['zen_dataportal_' .  $which_image]['settings'][$which_image . '_path'] = array(
          '#type' => 'textfield',
          '#title' => t('Path to custom') . ' ' . t($image_label),
          '#description' => t('The path to the file you would like to use as your') . ' ' . t($image_label),
          '#default_value' => $path,
  );
  $form['zen_dataportal_' .  $which_image]['settings'][$which_image . '_upload'] = array(
          '#type' => 'file',
          '#title' => t('Upload') . ' ' . t($image_label) . t(' image'),
          '#maxlength' => 40,
          '#description' => t("If you don't have direct file access to the server, use this field to upload your image.")
  );
}

/**
 *
 * Enter description here ...
 * @param $form_state
 *   A keyed array containing the current state of the form.
 * @param unknown_type $which_image
 */
function zen_dataportal_form_widget_image_submit(&$form_state, $which_image){
  // If the user uploaded a new image, save it to a permanent location
  // and use it in place of the default theme-provided file.
  $values = &$form_state['values'];
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
}

/**
*
* Enter description here ...
 * @param $form_state
 *   A keyed array containing the current state of the form.
* @param unknown_type $which_image
*/
function zen_dataportal_form_widget_image_validate(&$form_state, $which_image){
  $validators = array('file_validate_is_image' => array('gif', 'jpg', 'png'));
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