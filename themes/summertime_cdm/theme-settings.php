<?php
// $Id: theme-settings.php,v 1.1 2009/08/25 19:15:43 troy Exp $

/**
* Implementation of THEMEHOOK_settings() function.
*
* @param $saved_settings
*   array An array of saved settings for this theme.
* @return
*   array A form array.
*/
function phptemplate_settings($saved_settings) {

  $defaults = array(
    'admin_left_column' => 1,
    'admin_right_column' => 0,
		'banner_image' => 'cdm-header_3.jpg'
  );
  
  $settings = array_merge($defaults, $saved_settings);
   
   
  $form['tnt_container'] = array(
    '#type' => 'fieldset',
    '#title' => t('Column settings'),
    '#description' => t('Sometimes the content of admin section is much wider than the central column (especially on "views" and "theme" configuration pages), and as a result the content is cut. Here you can choose if you want the columns to be displayed in admin section, or not.'),
    '#collapsible' => TRUE,
    '#collapsed' => false,
  );
  
  // General Settings
  $form['tnt_container']['admin_left_column'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show left column in admin section'),
    '#default_value' => $settings['admin_left_column']
    );
  
  $form['tnt_container']['admin_right_column'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show right column in admin section'),
    '#default_value' => $settings['admin_right_column']
    ); 

	$form['edit_container'] = array(
    '#type' => 'fieldset',
    '#title' => t('Site Banner'),
    '#description' => t('Select the banner image to be displayed.'),
    '#collapsible' => TRUE,
    '#collapsed' => false,
  );
	
	// General Settings
  $form['edit_container']['banner_image'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Default options'),
		'#default_value' => $settings['banner_image'],
		'#options' => array(
			'cdm-platform-header.jpg' => t('CDM Platform'),
			'cdm-header_3.jpg' => t('CDM Setups')
		)
	);
  
  // Return theme settings form
  return $form;
}  

?>
