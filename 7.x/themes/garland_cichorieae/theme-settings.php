<?php
/**
 * see http://drupal.org/node/177868
 */
/**
* Implementation of THEMEHOOK_settings() function.
*
* @param $saved_settings
*   array An array of saved settings for this theme.
* @return
*   array A form array.
*/
function phptemplate_settings($saved_settings) {

    $key = sub_theme();

    $defaults = array(
        'default_banner_right' => 1,
        'banner_right_path' => '',
    );

  $settings = array_merge($defaults, $saved_settings);

  // Check for a new uploaded logo, and use that instead.
  if ($file = file_check_upload('banner_right_upload')) {
    if ($info = image_get_info($file->filepath)) {
      $parts = pathinfo($file->filename);
      $filename = ($key) ? str_replace('/', '_', $key) .'_banner_right.'. $parts['extension'] : '_banner_right.'. $parts['extension'];

      if ($file = file_save_upload('banner_right_upload', $filename, 1)) {
        $_POST['default_banner_right'] = 0;
        $_POST['banner_right_path'] = $file->filepath;
        $form["#after_build"][] = "banner_right_after_build";
      }
    }
    else {
      form_set_error('file_upload', t('Only JPEG, PNG and GIF images are allowed to be used as logos.'));
    }
  }

  if (true || $file) {
        $form['default_banner_right'] = array(
          '#type' => 'checkbox',
          '#title' => t('Use the right default header image'),
          '#default_value' => $settings['default_banner_right'],
          '#tree' => FALSE,
          '#description' => t('Check here if you want the theme to use the right header image supplied with it.')
         );

        $form['banner_right_path'] = array(
          '#type' => 'textfield',
          '#title' => t('Right banner image path'),
          '#default_value' => $settings['banner_right_path'],
          '#maxlength' => 255
          );

        $form['banner_right_upload'] = array(
          '#type' => 'file',
          '#title' => t('Upload new right banner image'),
          );
  }

  return $form;
}


function banner_right_after_build(&$form, &$form_values) {
  $form["#post"]["banner_right_path"] = $_POST['banner_right_path'];
  return $form;
}


