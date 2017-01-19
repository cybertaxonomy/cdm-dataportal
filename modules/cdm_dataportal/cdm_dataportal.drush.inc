<?php

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
  return $items;
}

/**
 * Callback for the drop-cdm-nodes command
 */
function drush_cdm_dataportal_drop_cdm_nodes(){
  cdm_delete_all_cdm_nodes();
  drush_print("All cdm related drupal nodes dropped");
}
