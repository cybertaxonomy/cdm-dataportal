<?php

define("POLYFILL_TEST_URL", "/polyfills/box-sizing-polyfill/boxsizing.htc");

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
  global $base_root;

  // Work-around for a core bug affecting admin themes. See issue #943212.
  if (isset($form_id)) {
    return;
  }

  $path_to_theme = drupal_get_path('theme', 'zen_dataportal');

  // check browser compatibility requirements
  $browser_compatibility_issues = array();
  $response = drupal_http_request($base_root . POLYFILL_TEST_URL, array("method"=>"HEAD"));
  // preprocess-css
  if($response->code != 200){
    $browser_compatibility_issues[] = 'The ' . l('polyfills folder' ,'/polyfills/', array('external'=>TRUE))
        .' is not found.'
        . 'Please read the installation instructions in the ' .l('README', $path_to_theme . '/README.txt') .' of this theme.';
  }
  if(variable_get('preprocess_css', 0) !== 1) {
    $browser_compatibility_issues[] = 'The '
        . l(t('Aggregate and compress CSS files'), 'admin/config/development/performance', array('fragment'=>'edit-preprocess-css'))
        . ' option must be turned on. ' . variable_get('preprocess_css', 0);
  }
  if(count($browser_compatibility_issues) > 0 ){
    drupal_set_message('<strong>Browser compatibility problems:</strong><ul><li>' . join('</li><li>', $browser_compatibility_issues) .
      '</li></ul>The theme will not be rendered correctly in InternetExplorer 8, 7 or 6 until these problems are solved.', 'error');
  }

  // will be available in the page.tpl.php
   $form['zen_dataportal_misc'] = array(
          '#type' => 'fieldset',
          '#title' => t('Miscellaneous settings'),
          '#description' => t('Miscellaneous settings for the DataPoral Zen theme.')
  );
  $form['zen_dataportal_misc']['site_name_position'] = array(
    '#type'          => 'select',
    '#title'         => t('Site name posistion'),
    '#description'   => t('The site name can be positioned over the banner or below the banner.'
        . '<br/><stong>NOTE:</strong> If your are choosing to position the site name below the banner you may also want to '
        . 'adapt the <em>site name color</em>.' ),
    '#options' => array(
      'hide' => t('Hidden'),
      'above_banner' => t('Above banner'),
      'below_banner' => t('Below banner'),
    ),
    '#default_value' => theme_get_setting('site_name_position'),
  );
  $form['zen_dataportal_misc']['header_margin_bottom'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Header margin at bottom'),
      '#default_value' => theme_get_setting('header_margin_bottom'),
      '#disabled'      => theme_get_setting('site_name_position') == 'below_banner',
      '#description'   => t('This margin instroduces space between the header and the main menu. This setting not applies if @site-name-pos" is set to "@below-banner'
          . 'The entered value can be any css length measurement. It is a <number> immediately followed by a length unit (px, em, pc, in, mm, …). See @link for more information',
            array(
                '@site-name-pos' => '<b>' . t('Site name posistion') . '</b>',
                '@below-banner' => '<b>' . t('Below banner') . '</b>',
                '@link' => l('https://developer.mozilla.org/en-US/docs/CSS/length', 'https://developer.mozilla.org/en-US/docs/CSS/length'),
             )
          ),
  );

  // TODO : site-name shadow: text-shadow: black 0 10px 20px, black 0 5px

  drupal_add_css($path_to_theme . '/js/colorpicker/css/colorpicker.css', 'file');
  drupal_add_js($path_to_theme. '/js/colorpicker/js/colorpicker.js', 'file');
  drupal_add_js($path_to_theme . '/js/settings-ui.js', 'file');
  $form['zen_dataportal_colors'] = array(
          '#type' => 'fieldset',
          '#title' => t('Colors'),
          '#description' => t('Configure colors where.'),
          '#attributes' => array('class' => array('theme-settings-bottom')),
  );
  zen_dataportal_form_widget_color(
      $form['zen_dataportal_colors'],
      'site-name_color',
      t('The color of the site name which is shown in the header.')
  );
  zen_dataportal_form_widget_color(
      $form['zen_dataportal_colors'],
      'main-menu_background-color',
      t('The color of the menu background.')
  );
  zen_dataportal_form_widget_color(
      $form['zen_dataportal_colors'],
      'main-menu_background-color-2',
      t('The second color of the menu background. If this is also set the menu background will have a grandiend.')
  );
  zen_dataportal_form_widget_color(
      $form['zen_dataportal_colors'],
      'sub-header_background-color',
      t('The background color sub-header.')
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
  if($values['default_logo']) {
    $logo_file = path_to_theme() . '/logo.png';
  } else {
    $logo_file = 'public://' . $values['logo_path'];
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
  $label = str_replace('_', ' ', $which_image);
  $form['zen_dataportal_' .  $which_image] = array(
          '#type' => 'fieldset',
          '#title' => ucfirst(t($label)) . ' ' . t('image settings'),
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
          '#title' => t('Path to custom') . ' ' . t($label),
          '#description' => t('The path to the file you would like to use as your') . ' ' . t($label),
          '#default_value' => $path,
  );
  $form['zen_dataportal_' .  $which_image]['settings'][$which_image . '_upload'] = array(
          '#type' => 'file',
          '#title' => t('Upload') . ' ' . t($label) . t(' image'),
          '#maxlength' => 40,
          '#description' => t("If you don't have direct file access to the server, use this field to upload your image.")
  );
}

/**
 *
 * Enter description here ...
 * @param $color_settings_key
 *
 */
function zen_dataportal_form_widget_color(&$form, $color_settings_key, $description) {
  $label = str_replace('_', ' ', $color_settings_key);
  $form['zen_dataportal_colors'][$color_settings_key] = array(
          '#type'          => 'textfield',
          '#title'         => ucfirst(t($label)),
          '#default_value' => theme_get_setting($color_settings_key),
          '#description'   => $description, //t('Set the color of the site name which is shown in the header. Must be a css color value like: #000000'),
          '#attributes' => array(
            'class' => array('color-picker'),
            'size' => '7',
          ),
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