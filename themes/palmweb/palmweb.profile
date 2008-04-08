function palmweb_profile_modules() {
    return array (
        0 => 'admin_menu',
        1 => 'block',
        2 => 'cdm_api',
        3 => 'jquery_update',
        4 => 'cdm_mediauri',
        5 => 'cdm_taxontree',
        6 => 'color',
        7 => 'comment',
        8 => 'devel',
        9 => 'devel_node_access',
        10 => 'filter',
        11 => 'help',
        12 => 'cdm_dataportal',
        13 => 'menu',
        14 => 'node',
        15 => 'profile_generator',
        16 => 'system',
        17 => 'taxonomy',
        18 => 'tinytax',
        19 => 'user',
        20 => 'watchdog',
      );
}


/************************************************************
*                          VARIABLES                        *
*               these seem to work fine - Dave              *
************************************************************/
    variable_set('admin_menu_display', '0');
    variable_set('admin_menu_show_all', 0);
    variable_set('admin_theme', 'bluemarine');
    variable_set('anonymous', 'Anonymous');
    variable_set('cache', '0');
    variable_set('cache_lifetime', '0');
    variable_set('cdm_secUuid_default', '1001-stub');
    variable_set('cdm_webservice_debug', 0);
    variable_set('cdm_webservice_isStub', 1);
    variable_set('cdm_webservice_proxy_port', '8080');
    variable_set('cdm_webservice_proxy_pwd', '');
    variable_set('cdm_webservice_proxy_url', 'proxy.ad.kew.org');
    variable_set('cdm_webservice_proxy_usr', '');
    variable_set('cdm_webservice_type', 'json');
    variable_set('cdm_webservice_url', 'http://dev.e-taxonomy.eu/svn/trunk/drupal/modules/cdm_dataportal/cdm_api/ws_stub/');
    variable_set('color_palmweb_files', array (
      0 => 'files/color/palmweb-2722b4e0/menu-collapsed.gif',
      1 => 'files/color/palmweb-2722b4e0/menu-expanded.gif',
      2 => 'files/color/palmweb-2722b4e0/menu-leaf.gif',
      3 => 'files/color/palmweb-2722b4e0/body.png',
      4 => 'files/color/palmweb-2722b4e0/bg-bar.png',
      5 => 'files/color/palmweb-2722b4e0/bg-bar-white.png',
      6 => 'files/color/palmweb-2722b4e0/bg-tab.png',
      7 => 'files/color/palmweb-2722b4e0/bg-navigation.png',
      8 => 'files/color/palmweb-2722b4e0/bg-content-left.png',
      9 => 'files/color/palmweb-2722b4e0/bg-content-right.png',
      10 => 'files/color/palmweb-2722b4e0/bg-content.png',
      11 => 'files/color/palmweb-2722b4e0/bg-navigation-item.png',
      12 => 'files/color/palmweb-2722b4e0/bg-navigation-item-hover.png',
      13 => 'files/color/palmweb-2722b4e0/gradient-inner.png',
      14 => 'files/color/palmweb-2722b4e0/logo.png',
      15 => 'files/color/palmweb-2722b4e0/screenshot.png',
      16 => 'files/color/palmweb-2722b4e0/style.css',
    ));
    variable_set('color_palmweb_logo', 'files/color/palmweb-2722b4e0/logo.png');
    variable_set('color_palmweb_palette', array (
      'base' => '#8cb668',
      'link' => '#027ac6',
      'top' => '#2385c2',
      'bottom' => '#5ab5ee',
      'text' => '#494949',
    ));
    variable_set('color_palmweb_screenshot', 'files/color/palmweb-2722b4e0/screenshot.png');
    variable_set('color_palmweb_stylesheet', 'files/color/palmweb-2722b4e0/style.css');
    variable_set('comment_page', 0);
    variable_set('devel_error_handler', '1');
    variable_set('devel_execution', '5');
    variable_set('devel_form_weights', 0);
    variable_set('devel_query_display', 0);
    variable_set('devel_query_sort', '0');
    variable_set('devel_redirect_page', 0);
    variable_set('devel_store_queries', 0);
    variable_set('devel_store_random', '1');
    variable_set('dev_mem', 0);
    variable_set('dev_query', 0);
    variable_set('dev_timer', 1);
    variable_set('file_directory_temp', 'C:\\Users\\ADM-D~1.TAY\\AppData\\Local\\Temp\\php\\upload');
    variable_set('filter_html_1', 1);
    variable_set('menu_primary_menu', 2);
    variable_set('menu_secondary_menu', 2);
    variable_set('node_options_forum', array (
      0 => 'status',
    ));
    variable_set('node_options_page', array (
      0 => 'status',
    ));
    variable_set('preprocess_css', '0');
    variable_set('site_footer', '');
    variable_set('site_frontpage', 'node/1');
    variable_set('site_mail', 'd.taylor@kew.org');
    variable_set('site_mission', '');
    variable_set('site_name', ' p a l m W E B');
    variable_set('site_offline', '0');
    variable_set('site_offline_message', 'palmWEB is currently offline for maintenance. Thank you for your patience.');
    variable_set('site_slogan', '');
    variable_set('smtp_library', '');
    system_theme_data();
    db_query("UPDATE {system} SET status = 1 WHERE type = 'theme' and name = '%s'", 'palmweb');
    variable_set('theme_default', 'palmweb');

    variable_set('tinytax_cdm_block', '1');
    variable_set('tinytax_cdm_block_enabel', 1);


/************************************************************
*                       BLOCK SETTINGS                      *
*          to form a basis for discussion! - Dave           *
*************************************************************/

    db_query("INSERT INTO {boxes} (bid, body, info, format) VALUES (%d, '%s', '%s', '%s')",
        '1',
        '<img src="http://localhost/drupal-5.7/sites/default/themes/palmweb/coverimg.jpg"/>',
        'quick image block',
        '3'
    );

    db_query(

        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'block', '1', 'palmweb', '1', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'comment', '0', 'palmweb', '0', '0', '', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'menu', $menu['2'], 'palmweb', '0', '0', '', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'node', '0', 'palmweb', '0', '0', '', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '0', 'palmweb', '1', '0', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '1', 'palmweb', '1', '0', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '2', 'palmweb', '0', '0', '', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '3', 'palmweb', '0', '0', '', '0', '0', '0', '', ''
    );

    db_query(
        "DELETE FROM blocks WHERE module = "cdm_dataportal" and theme = "palmweb");
    
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '0', 'palmweb', '1', '-5', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '1', 'palmweb', '1', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '2', 'palmweb', '1', '-7', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '3', 'palmweb', '1', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'tinytax', 'tinytax_cdm_block', 'palmweb', '1', '-6', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'devel_node_access', '0', 'palmweb', '0', '0', '', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'devel', '0', 'palmweb', '0', '0', '', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'devel', '1', 'palmweb', '1', '9', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'devel', '2', 'palmweb', '0', '0', '', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks_roles} (module,delta,rid) VALUES ('%s', '%s', %d)",
        'tinytax', 'tinytax_cdm_block', $role_id['anonymous user']
    );
    db_query(
        "INSERT INTO {blocks_roles} (module,delta,rid) VALUES ('%s', '%s', %d)",
        'tinytax', 'tinytax_cdm_block', $role_id['authenticated user']
    );