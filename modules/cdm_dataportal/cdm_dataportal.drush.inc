<?php

require_once("lib/class.Diff.php");
require_once("classes/NameRenderConfiguration.php");

/**
 * @file
 * Drush integration for the devel module.
 */

/**
 * Implements hook_drush_command().
 */
function cdm_dataportal_drush_command() {
  $items['drop-cdm-nodes'] = array(
    'description' => dt('Drops all drupal nodes created for cdm content.'),
  );
  $items['cdm-render-templates-config-status'] = array(
    'description' => dt('Reports the status of the drupal variable ' . NameRenderConfiguration::CDM_NAME_RENDER_TEMPLATES . ', whether DEFAULT, CUSTOM, or PRE_380'),
    'aliases' => ['cdm-rtcs'],
  );
  $items['cdm-name-part-config-status'] = array(
    'description' => dt('Reports the status of the drupal variable ' . NameRenderConfiguration::CDM_PART_DEFINITIONS . ', whether DEFAULT, CUSTOM, or PRE_380'),
    'aliases' => ['cdm-npcs'],
  );
  $items['cdm-render-templates-config-diff'] = array(
    'description' => dt('Provides the diff of the default and the actual value of the drupal variable ' . NameRenderConfiguration::CDM_NAME_RENDER_TEMPLATES),
    'aliases' => ['cdm-rtcd'],
  );
  $items['cdm-name-part-config-diff'] = array(
    'description' => dt('Provides the diff of the default and the actual value of the drupal variable ' . NameRenderConfiguration::CDM_PART_DEFINITIONS),
    'aliases' => ['cdm-npcd'],
  );
  return $items;
}

/**
 * Callback for the drop-cdm-nodes command
 */
function drush_cdm_dataportal_drop_cdm_nodes(){
  cdm_delete_all_cdm_nodes();
  drush_print("All cdm related drupal nodes dropped");
}

/**
 * Callback for the 'cdm-render-templates-config-status' command
 */
function drush_cdm_dataportal_cdm_render_templates_config_status(){
  $nameRenderConfiguration = new NameRenderConfiguration();
  $config_status = $nameRenderConfiguration->nameRenderTemplateConfigurationStatus();
  $status_label = status_label($config_status);
  drush_print($status_label);
}

/**
 * Callback for the 'cdm-name-part-config-status' command
 */
function drush_cdm_dataportal_cdm_name_part_config_status(){
  $nameRenderConfiguration = new NameRenderConfiguration();
  $config_status = $nameRenderConfiguration->partDefinitionConfigurationStatus();
  $status_label = status_label($config_status);
  drush_print($status_label);
}

/**
 * Callback for the 'cdm-render-templates-config-diff' command
 */
function drush_cdm_dataportal_cdm_render_templates_config_diff(){
  $nameRenderConfiguration = new NameRenderConfiguration();
  $config_status = $nameRenderConfiguration->nameRenderTemplateConfigurationStatus();
  if($config_status == NameRenderConfiguration::CUSTOM_CONFIGURATION){
    $diff = Diff::compare($nameRenderConfiguration->getDefaultRenderTemplatesJson(), $nameRenderConfiguration->getCurrentRenderTemplatesJson());
    drush_print(Diff::toString($diff));
  }
}

/**
 * Callback for the 'cdm-render-templates-config-diff' command
 */
function drush_cdm_dataportal_cdm_name_part_config_diff(){
  $nameRenderConfiguration = new NameRenderConfiguration();
  $config_status = $nameRenderConfiguration->partDefinitionConfigurationStatus();
  if($config_status == NameRenderConfiguration::CUSTOM_CONFIGURATION){
    $diff = Diff::compare($nameRenderConfiguration->getDefaultPartDefinitionJson(), $nameRenderConfiguration->getCurrentPartDefinitionJson());
    drush_print(Diff::toString($diff));
  }
}

/**
 * @param $config_status
 *
 * @return string
 */
function status_label($config_status) {
  $status_label = '';
  switch ($config_status) {
    case NameRenderConfiguration::PRE380_CONFIGURATION:
      $status_label = 'PRE_380';
      break;
    case NameRenderConfiguration::CUSTOM_CONFIGURATION:
      $status_label = 'CUSTOM';
      break;
    case NameRenderConfiguration::DEFAULT_CONFIGURATION:
    default:
      $status_label = 'DEFAULT';
  }
  return $status_label;
}
