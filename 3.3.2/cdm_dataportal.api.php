<?php

/**
 * @file
 * Hooks provided by the CDM DataPortal module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Alter the feature toc items.
 *
 * Modules implementing this hook can add, remove or modify
 * the items in the table of contents of features.
 *
 * the items array is an array suitable for theme_item_list().
 *
 */
function hook_cdm_feature_node_toc_items_alter($items) {

  // NOTE: drupal_get_query_parameters has no internal static cache variable
  $http_request_params = drupal_get_query_parameters();

  $items[] = array(
              // a html link element as item:
              l(
                theme('cdm_feature_name', array('feature_name' => 'My new feature item')),
                $_GET['q'],
                array(
                    'attributes' => array('class' => array('toc')),
                    'fragment' => generalizeString('My new feature item'),
                    'query' => $http_request_params
                )
              )
            );

  return $items;
}

/**
 * @} End of "addtogroup hooks".
 */