<?php

// vim: filetype=php

/************************************************************
*                           MODULES                         *
************************************************************/
function CDM_DataPortal_profile_modules() {
    return array (
        0 => 'admin_menu',
        1 => 'block',
        2 => 'cdm_api',
        3 => 'jquery_update',
        4 => 'taxonomy',
        5 => 'color',
        6 => 'comment',
        7 => 'cdm_taxontree',
        8 => 'filter',
        9 => 'help',
        10 => 'cdm_dataportal',
        11 => 'menu',
        12 => 'node',
        13 => 'profile_generator',
        14 => 'system',
        15 => 'ext_links',
        16 => 'user',
        17 => 'watchdog',
      );
}

/************************************************************
*                           DETAILS                         *
************************************************************/
function CDM_DataPortal_profile_details() {
    return array (
        'name' => 'CDM DataPortal',
        'description' => 'CDM DataPortal installation profile generated automatically on 28th Sep 2010 02:05pm',
      );
}

function CDM_DataPortal_profile_final() {
/************************************************************
*                          VARIABLES                        *
************************************************************/
    variable_set('anonymous', 'Anonymous');
    variable_set('comment_page', 0);
    variable_set('file_directory_temp', 'c:\\windows\\temp');
    variable_set('filter_html_1', 1);
    variable_set('menu_primary_menu', 2);
    variable_set('menu_secondary_menu', 2);
    variable_set('node_options_forum', array (
      0 => 'status',
    ));
    variable_set('node_options_page', array (
      0 => 'status',
    ));
    variable_set('site_footer', '');
    variable_set('site_frontpage', 'node/1');
    variable_set('site_mail', '');
    variable_set('site_mission', '');
    variable_set('site_name', 'CDM Dataportal');
    variable_set('site_slogan', '');
    system_theme_data();
    db_query("UPDATE {system} SET status = 1 WHERE type = 'theme' and name = '%s'", 'garland');
    variable_set('theme_default', 'garland');
    variable_set('theme_settings', array (
      'toggle_node_info_page' => false,
      'toggle_node_info_cdm_media' => false,
      'toggle_node_info_cdm_name' => false,
      'toggle_node_info_cdm_taxon' => false,
      'toggle_node_info_cdm_reference' => false
    ));
    variable_set('cdm_webservice_debug', 0);
    variable_set('cdm_webservice_cache', 0);
    variable_set('distribution_sort', 'HIDE_TDWG2');
    

/************************************************************
*                            ROLES                          *
************************************************************/
    $role_id['anonymous user'] = 1;
    $role_id['authenticated user'] = 2;

/************************************************************
*                            USERS                          *
************************************************************/
    $user = user_save(new stdClass(), array (
        'name' => 'admin',
        'mail' => 'admin@dataportal.net',
        'mode' => '0',
        'sort' => '0',
        'threshold' => '0',
        'theme' => '',
        'signature' => '',
        'created' => '1285671981',
        'access' => '1285675547',
        'login' => '0',
        'status' => '1',
        'timezone' => '0',
        'language' => '',
        'picture' => '',
        'init' => 'wp5Admin@bgbm.org',
        'data' => 'a:0:{}',
      ));
    db_query("UPDATE {users} SET pass='%s' WHERE uid=%d", '21232f297a57a5a743894a0e4a801fc3', $user->uid);
    $user_id['admin'] = $user->uid;

/************************************************************
*                   USERS <=> ROLES MAPPING                 *
************************************************************/

/************************************************************
*                       EXPORTING NODES                     *
************************************************************/
    // exporting nodes of type: Page
    db_query(
        "INSERT INTO {node} (nid,vid,type,title,uid,status,created,changed,comment,promote,moderate,sticky)
        VALUES ('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",
        '1','1','page','CDM DataPortal','1','1','1285672677','1285673272','0','0','0','0'
    );
    db_query(
        "INSERT INTO {node_revisions} (nid,vid,uid,title,body,teaser,log,timestamp,format)
        VALUES ('%s','%s','%s','%s','%s','%s','%s','%s','%s')",
        '1','1','1','CDM DataPortal','<h3>Welcome to your CDM DataPortal. </h3>
    
    This data portal makes full use of the Common Data Model  (CDM), a central component of the Internet Platform for Cybertaxonomy being developed by EDIT workpackage 5. All taxon pages are created as dynamic web pages from the underlying database.
    
    In order to finish the setup of your DataPortal please visit the <?php print(l(\'CDM Dataportal settings\', \'admin/settings/cdm_dataportal\')); ?>. The CDM Dataportal provides several Drupal Blocks (<i>taxon search</i>, <i>classification browser</i>, <i>external links</i>, <i>print this page</i>) which you may want to activate in the <?php print(l(\'Blocks Settings\', \'admin/build/block\')); ?>','<h3>Welcome to your CDM DataPortal. </h3>
    
    This data portal makes full use of the Common Data Model  (CDM), a central component of the Internet Platform for Cybertaxonomy being developed by EDIT workpackage 5. All taxon pages are created as dynamic web pages from the underlying database.
    
    In order to finish the setup of your DataPortal please visit the <?php print(l(\'CDM Dataportal settings\', \'admin/settings/cdm_dataportal\')); ?>. The CDM Dataportal provides several Drupal Blocks (<i>taxon search</i>, <i>classification browser</i>, <i>external links</i>, <i>print this page</i>) which you may want to activate in the <?php print(l(\'Blocks Settings\', \'admin/build/block\')); ?>','','1285673272','2'
    );

    system_initialize_theme_blocks('garland');
    
    // set site info
    //  * site name
    //  * start page
    db_query(
        "REPLACE INTO {variable} (name, value) VALUES
          ('cdm_dataportal_geoservice_display_width', 's:3:\"600\";'),
          ('site_frontpage', 's:6:\"node/1\";'),
          ('site_name', 's:9:\"TDWG 2010\";')
        ");
        
   // FIX node sequences !!!!!
   
    db_query(
        "REPLACE INTO {sequences} (name, id) VALUES
        ('{node_nid}', 1),
        ('{node_revisions_vid}', 1);
        ");
	
    return;
}

?>
