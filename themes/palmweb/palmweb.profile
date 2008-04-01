<?php

// vim: filetype=php

/************************************************************
*                           MODULES                         *
************************************************************/
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
*                           DETAILS                         *
************************************************************/
function palmweb_profile_details() {
    return array (
        'name' => 'palmweb',
        'description' => 'Installation profile generated automatically on 20th Mar 2008 05:48pm',
      );
}

function palmweb_profile_final() {
/************************************************************
*                          VARIABLES                        *
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
    variable_set('theme_garland_settings', array (
      'toggle_logo' => 1,
      'toggle_name' => 1,
      'toggle_slogan' => 0,
      'toggle_mission' => 1,
      'toggle_node_user_picture' => 0,
      'toggle_comment_user_picture' => 0,
      'toggle_search' => 0,
      'toggle_favicon' => 1,
      'default_logo' => 1,
      'logo_path' => '',
      'logo_upload' => '',
      'default_favicon' => 1,
      'favicon_path' => '',
      'favicon_upload' => '',
      'op' => 'Save configuration',
      'form_token' => '5ba2656f622a9b966365e36cec2dff6e',
      'scheme' => '#0072b9,#027ac6,#2385c2,#5ab5ee,#494949',
      'palette' => 
      array (
        'base' => '#0072b9',
        'link' => '#027ac6',
        'top' => '#2385c2',
        'bottom' => '#5ab5ee',
        'text' => '#494949',
      ),
      'theme' => 'garland',
      'info' => 
      array (
        'schemes' => 
        array (
          '#0072b9,#027ac6,#2385c2,#5ab5ee,#494949' => 'Blue Lagoon (Default)',
          '#464849,#2f416f,#2a2b2d,#5d6779,#494949' => 'Ash',
          '#55c0e2,#000000,#085360,#007e94,#696969' => 'Aquamarine',
          '#d5b048,#6c420e,#331900,#971702,#494949' => 'Belgian Chocolate',
          '#3f3f3f,#336699,#6598cb,#6598cb,#000000' => 'Bluemarine',
          '#d0cb9a,#917803,#efde01,#e6fb2d,#494949' => 'Citrus Blast',
          '#0f005c,#434f8c,#4d91ff,#1a1575,#000000' => 'Cold Day',
          '#c9c497,#0c7a00,#03961e,#7be000,#494949' => 'Greenbeam',
          '#ffe23d,#a9290a,#fc6d1d,#a30f42,#494949' => 'Mediterrano',
          '#788597,#3f728d,#a9adbc,#d4d4d4,#707070' => 'Mercury',
          '#5b5fa9,#5b5faa,#0a2352,#9fa8d5,#494949' => 'Nocturnal',
          '#7db323,#6a9915,#b5d52a,#7db323,#191a19' => 'Olivia',
          '#12020b,#1b1a13,#f391c6,#f41063,#898080' => 'Pink Plastic',
          '#b7a0ba,#c70000,#a1443a,#f21107,#515d52' => 'Shiny Tomato',
          '#18583d,#1b5f42,#34775a,#52bf90,#2d2d2d' => 'Teal Top',
          '' => 'Custom',
        ),
        'copy' => 
        array (
          0 => 'images/menu-collapsed.gif',
          1 => 'images/menu-expanded.gif',
          2 => 'images/menu-leaf.gif',
        ),
        'gradient' => 
        array (
          0 => 0,
          1 => 37,
          2 => 760,
          3 => 121,
        ),
        'fill' => 
        array (
          'base' => 
          array (
            0 => 0,
            1 => 0,
            2 => 760,
            3 => 568,
          ),
          'link' => 
          array (
            0 => 107,
            1 => 533,
            2 => 41,
            3 => 23,
          ),
        ),
        'slices' => 
        array (
          'images/body.png' => 
          array (
            0 => 0,
            1 => 37,
            2 => 1,
            3 => 280,
          ),
          'images/bg-bar.png' => 
          array (
            0 => 202,
            1 => 530,
            2 => 76,
            3 => 14,
          ),
          'images/bg-bar-white.png' => 
          array (
            0 => 202,
            1 => 506,
            2 => 76,
            3 => 14,
          ),
          'images/bg-tab.png' => 
          array (
            0 => 107,
            1 => 533,
            2 => 41,
            3 => 23,
          ),
          'images/bg-navigation.png' => 
          array (
            0 => 0,
            1 => 0,
            2 => 7,
            3 => 37,
          ),
          'images/bg-content-left.png' => 
          array (
            0 => 40,
            1 => 117,
            2 => 50,
            3 => 352,
          ),
          'images/bg-content-right.png' => 
          array (
            0 => 510,
            1 => 117,
            2 => 50,
            3 => 352,
          ),
          'images/bg-content.png' => 
          array (
            0 => 299,
            1 => 117,
            2 => 7,
            3 => 200,
          ),
          'images/bg-navigation-item.png' => 
          array (
            0 => 32,
            1 => 37,
            2 => 17,
            3 => 12,
          ),
          'images/bg-navigation-item-hover.png' => 
          array (
            0 => 54,
            1 => 37,
            2 => 17,
            3 => 12,
          ),
          'images/gradient-inner.png' => 
          array (
            0 => 646,
            1 => 307,
            2 => 112,
            3 => 42,
          ),
          'logo.png' => 
          array (
            0 => 622,
            1 => 51,
            2 => 64,
            3 => 73,
          ),
          'screenshot.png' => 
          array (
            0 => 0,
            1 => 37,
            2 => 400,
            3 => 240,
          ),
        ),
        'blend_target' => '#ffffff',
        'preview_image' => 'color/preview.png',
        'preview_css' => 'color/preview.css',
        'base_image' => 'color/base.png',
      ),
    ));
    variable_set('theme_palmweb_orig_settings', array (
      'toggle_logo' => 1,
      'toggle_name' => 1,
      'toggle_slogan' => 0,
      'toggle_mission' => 1,
      'toggle_node_user_picture' => 0,
      'toggle_comment_user_picture' => 0,
      'toggle_search' => 0,
      'toggle_favicon' => 1,
      'default_logo' => 1,
      'logo_path' => '',
      'logo_upload' => '',
      'default_favicon' => 1,
      'favicon_path' => '',
      'favicon_upload' => '',
      'op' => 'Save configuration',
      'form_token' => '651d16aea6de8c23e38df3a3c141910e',
    ));
    variable_set('theme_palmweb_settings', array (
      'toggle_logo' => 0,
      'toggle_name' => 1,
      'toggle_slogan' => 0,
      'toggle_mission' => 1,
      'toggle_node_user_picture' => 0,
      'toggle_comment_user_picture' => 0,
      'toggle_search' => 0,
      'toggle_favicon' => 1,
      'default_logo' => 1,
      'logo_path' => '',
      'logo_upload' => '',
      'default_favicon' => 1,
      'favicon_path' => '',
      'favicon_upload' => '',
      'op' => 'Save configuration',
      'form_token' => '5ba2656f622a9b966365e36cec2dff6e',
      'scheme' => '',
      'palette' => 
      array (
        'base' => '#8cb668',
        'link' => '#027ac6',
        'top' => '#2385c2',
        'bottom' => '#5ab5ee',
        'text' => '#494949',
      ),
      'theme' => 'palmweb',
      'info' => 
      array (
        'schemes' => 
        array (
          '#0072b9,#027ac6,#2385c2,#5ab5ee,#494949' => 'Blue Lagoon (Default)',
          '#464849,#2f416f,#2a2b2d,#5d6779,#494949' => 'Ash',
          '#55c0e2,#000000,#085360,#007e94,#696969' => 'Aquamarine',
          '#d5b048,#6c420e,#331900,#971702,#494949' => 'Belgian Chocolate',
          '#3f3f3f,#336699,#6598cb,#6598cb,#000000' => 'Bluemarine',
          '#d0cb9a,#917803,#efde01,#e6fb2d,#494949' => 'Citrus Blast',
          '#0f005c,#434f8c,#4d91ff,#1a1575,#000000' => 'Cold Day',
          '#c9c497,#0c7a00,#03961e,#7be000,#494949' => 'Greenbeam',
          '#ffe23d,#a9290a,#fc6d1d,#a30f42,#494949' => 'Mediterrano',
          '#788597,#3f728d,#a9adbc,#d4d4d4,#707070' => 'Mercury',
          '#5b5fa9,#5b5faa,#0a2352,#9fa8d5,#494949' => 'Nocturnal',
          '#7db323,#6a9915,#b5d52a,#7db323,#191a19' => 'Olivia',
          '#12020b,#1b1a13,#f391c6,#f41063,#898080' => 'Pink Plastic',
          '#b7a0ba,#c70000,#a1443a,#f21107,#515d52' => 'Shiny Tomato',
          '#18583d,#1b5f42,#34775a,#52bf90,#2d2d2d' => 'Teal Top',
          '' => 'Custom',
        ),
        'copy' => 
        array (
          0 => 'images/menu-collapsed.gif',
          1 => 'images/menu-expanded.gif',
          2 => 'images/menu-leaf.gif',
        ),
        'gradient' => 
        array (
          0 => 0,
          1 => 37,
          2 => 760,
          3 => 121,
        ),
        'fill' => 
        array (
          'base' => 
          array (
            0 => 0,
            1 => 0,
            2 => 760,
            3 => 568,
          ),
          'link' => 
          array (
            0 => 107,
            1 => 533,
            2 => 41,
            3 => 23,
          ),
        ),
        'slices' => 
        array (
          'images/body.png' => 
          array (
            0 => 0,
            1 => 37,
            2 => 1,
            3 => 280,
          ),
          'images/bg-bar.png' => 
          array (
            0 => 202,
            1 => 530,
            2 => 76,
            3 => 14,
          ),
          'images/bg-bar-white.png' => 
          array (
            0 => 202,
            1 => 506,
            2 => 76,
            3 => 14,
          ),
          'images/bg-tab.png' => 
          array (
            0 => 107,
            1 => 533,
            2 => 41,
            3 => 23,
          ),
          'images/bg-navigation.png' => 
          array (
            0 => 0,
            1 => 0,
            2 => 7,
            3 => 37,
          ),
          'images/bg-content-left.png' => 
          array (
            0 => 40,
            1 => 117,
            2 => 50,
            3 => 352,
          ),
          'images/bg-content-right.png' => 
          array (
            0 => 510,
            1 => 117,
            2 => 50,
            3 => 352,
          ),
          'images/bg-content.png' => 
          array (
            0 => 299,
            1 => 117,
            2 => 7,
            3 => 200,
          ),
          'images/bg-navigation-item.png' => 
          array (
            0 => 32,
            1 => 37,
            2 => 17,
            3 => 12,
          ),
          'images/bg-navigation-item-hover.png' => 
          array (
            0 => 54,
            1 => 37,
            2 => 17,
            3 => 12,
          ),
          'images/gradient-inner.png' => 
          array (
            0 => 646,
            1 => 307,
            2 => 112,
            3 => 42,
          ),
          'logo.png' => 
          array (
            0 => 622,
            1 => 51,
            2 => 64,
            3 => 73,
          ),
          'screenshot.png' => 
          array (
            0 => 0,
            1 => 37,
            2 => 400,
            3 => 240,
          ),
        ),
        'blend_target' => '#ffffff',
        'preview_image' => 'color/preview.png',
        'preview_css' => 'color/preview.css',
        'base_image' => 'color/base.png',
      ),
    ));
    variable_set('theme_settings', array (
      'toggle_node_info_page' => false,
    ));
    variable_set('tinytax_cdm_block', '1');
    variable_set('tinytax_cdm_block_enabel', 1);
    variable_set('update_access_fixed', true);

/************************************************************
*                         NODE TYPES                        *
************************************************************/
    db_query("INSERT INTO {node_type} (type, name, module, description, help, has_title, title_label, has_body, body_label, min_word_count, custom, modified, locked, orig_type)
               VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')",
        'page','Page','node','If you want to add a static page, like a contact page or an about page, use a page.','','1','Title','1','Body','0','1','1','0','page'
    );    db_query("INSERT INTO {node_type} (type, name, module, description, help, has_title, title_label, has_body, body_label, min_word_count, custom, modified, locked, orig_type)
               VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')",
        'page','Page','node','If you want to add a static page, like a contact page or an about page, use a page.','','1','Title','1','Body','0','1','1','0','page','story','Story','node','Stories are articles in their simplest form: they have a title, a teaser and a body, but can be extended by other modules. The teaser is part of the body too. Stories may be used as a personal blog or for news articles.','','1','Title','1','Body','0','1','1','0','story'
    );
/************************************************************
*                            ROLES                          *
************************************************************/
    $role_id['anonymous user'] = 1;
    $role_id['authenticated user'] = 2;

/************************************************************
*                            USERS                          *
************************************************************/
    $user = user_save(new stdClass(), array (
        'name' => 'daveadmin',
        'mail' => 'd.taylor@kew.org',
        'mode' => '0',
        'sort' => '0',
        'threshold' => '0',
        'theme' => '',
        'signature' => '',
        'created' => '1203348986',
        'access' => '1206035406',
        'login' => '1206035301',
        'status' => '1',
        'timezone' => '0',
        'language' => '',
        'picture' => '',
        'init' => 'd.taylor@kew.org',
        'data' => 'a:0:{}',
      ));
    db_query("UPDATE {users} SET pass='%s' WHERE uid=%d", 'f93135e07834e2692f915c8f8754788f', $user->uid);
    $user_id['daveadmin'] = $user->uid;

/************************************************************
*                   USERS <=> ROLES MAPPING                 *
************************************************************/

/************************************************************
*                            MENUS                          *
************************************************************/

    while ( db_next_id('{menu}_mid') < 2) {}

    // first the primary links
    palmweb_profile_install_menu(2, array (
      0 => 
      array (
        'path' => 'node/4',
        'title' => 'About Content',
        'description' => '',
        'weight' => '1',
        'type' => '118',
        'children' => 
        array (
        ),
      ),
      1 => 
      array (
        'path' => 'node/3',
        'title' => 'Participants',
        'description' => '',
        'weight' => '0',
        'type' => '118',
        'children' => 
        array (
        ),
      ),
      2 => 
      array (
        'path' => '<front>',
        'title' => 'Home',
        'description' => '',
        'weight' => '-1',
        'type' => '118',
        'children' => 
        array (
        ),
      ),
    ));
    palmweb_profile_install_menu(0, array (
    ));

/************************************************************
*                         URL ALIASES                       *
************************************************************/

/************************************************************
*                           BLOCKS                          *
************************************************************/

    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '0', 'garland', '1', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '1', 'garland', '1', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'block', '1', 'chameleon', '1', '0', 'left', '0', '0', '0', '', ''
    );
    db_query("INSERT INTO {boxes} (bid, body, info, format) VALUES (%d, '%s', '%s', '%s')",
        '1',
        '<img src="http://localhost/drupal-5.7/sites/default/themes/palmweb/coverimg.jpg"/>',
        'quick image block',
        '3'
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'comment', '0', 'chameleon', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'menu', $menu['2'], 'chameleon', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'node', '0', 'chameleon', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '0', 'chameleon', '1', '0', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '1', 'chameleon', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '2', 'chameleon', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '3', 'chameleon', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '0', 'chameleon', '1', '0', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '1', 'chameleon', '1', '0', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'devel', '0', 'chameleon', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'devel', '1', 'chameleon', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'devel', '2', 'chameleon', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'block', '1', 'expertsdb', '1', '0', 'left', '0', '0', '0', '', ''
    );
    db_query("INSERT INTO {boxes} (bid, body, info, format) VALUES (%d, '%s', '%s', '%s')",
        '1',
        '<img src="http://localhost/drupal-5.7/sites/default/themes/palmweb/coverimg.jpg"/>',
        'quick image block',
        '3'
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'comment', '0', 'expertsdb', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'menu', $menu['2'], 'expertsdb', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'node', '0', 'expertsdb', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '0', 'expertsdb', '1', '0', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '1', 'expertsdb', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '2', 'expertsdb', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '3', 'expertsdb', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '0', 'expertsdb', '1', '0', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '1', 'expertsdb', '1', '0', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'devel', '0', 'expertsdb', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'devel', '1', 'expertsdb', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'devel', '2', 'expertsdb', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'block', '1', 'bluemarine', '1', '0', 'left', '0', '0', '0', '', ''
    );
    db_query("INSERT INTO {boxes} (bid, body, info, format) VALUES (%d, '%s', '%s', '%s')",
        '1',
        '<img src="http://localhost/drupal-5.7/sites/default/themes/palmweb/coverimg.jpg"/>',
        'quick image block',
        '3'
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'comment', '0', 'bluemarine', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'menu', $menu['2'], 'bluemarine', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'node', '0', 'bluemarine', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '0', 'bluemarine', '1', '0', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '1', 'bluemarine', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '2', 'bluemarine', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '3', 'bluemarine', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '0', 'bluemarine', '1', '0', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '1', 'bluemarine', '1', '0', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '2', 'bluemarine', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'devel', '0', 'bluemarine', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'devel', '1', 'bluemarine', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'devel', '2', 'bluemarine', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'block', '1', 'zen_EDIT', '1', '0', 'left', '0', '0', '0', '', ''
    );
    db_query("INSERT INTO {boxes} (bid, body, info, format) VALUES (%d, '%s', '%s', '%s')",
        '1',
        '<img src="http://localhost/drupal-5.7/sites/default/themes/palmweb/coverimg.jpg"/>',
        'quick image block',
        '3'
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'comment', '0', 'zen_EDIT', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'menu', $menu['2'], 'zen_EDIT', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'node', '0', 'zen_EDIT', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '0', 'zen_EDIT', '1', '0', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '1', 'zen_EDIT', '1', '0', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '2', 'zen_EDIT', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '3', 'zen_EDIT', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '0', 'zen_EDIT', '1', '-5', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '1', 'zen_EDIT', '1', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '2', 'zen_EDIT', '1', '-7', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '3', 'zen_EDIT', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'tinytax', 'tinytax_cdm_block', 'zen_EDIT', '1', '-6', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'devel_node_access', '0', 'zen_EDIT', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'devel', '0', 'zen_EDIT', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'devel', '1', 'zen_EDIT', '1', '9', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'devel', '2', 'zen_EDIT', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'block', '1', 'garland_PALM', '1', '0', 'left', '0', '0', '0', '', ''
    );
    db_query("INSERT INTO {boxes} (bid, body, info, format) VALUES (%d, '%s', '%s', '%s')",
        '1',
        '<img src="http://localhost/drupal-5.7/sites/default/themes/palmweb/coverimg.jpg"/>',
        'quick image block',
        '3'
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'comment', '0', 'garland_PALM', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'menu', $menu['2'], 'garland_PALM', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'node', '0', 'garland_PALM', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '0', 'garland_PALM', '1', '0', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '1', 'garland_PALM', '1', '0', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '2', 'garland_PALM', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '3', 'garland_PALM', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '0', 'garland_PALM', '1', '-5', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '1', 'garland_PALM', '1', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '2', 'garland_PALM', '1', '-7', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '3', 'garland_PALM', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'tinytax', 'tinytax_cdm_block', 'garland_PALM', '1', '-6', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'devel_node_access', '0', 'garland_PALM', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'devel', '0', 'garland_PALM', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'devel', '1', 'garland_PALM', '1', '9', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'devel', '2', 'garland_PALM', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'block', '1', 'palmweb_orig', '1', '0', 'left', '0', '0', '0', '', ''
    );
    db_query("INSERT INTO {boxes} (bid, body, info, format) VALUES (%d, '%s', '%s', '%s')",
        '1',
        '<img src="http://localhost/drupal-5.7/sites/default/themes/palmweb/coverimg.jpg"/>',
        'quick image block',
        '3'
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'comment', '0', 'palmweb_orig', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'menu', $menu['2'], 'palmweb_orig', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'node', '0', 'palmweb_orig', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '0', 'palmweb_orig', '1', '0', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '1', 'palmweb_orig', '1', '0', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '2', 'palmweb_orig', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '3', 'palmweb_orig', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '0', 'palmweb_orig', '1', '-5', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '1', 'palmweb_orig', '1', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '2', 'palmweb_orig', '1', '-7', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '3', 'palmweb_orig', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'tinytax', 'tinytax_cdm_block', 'palmweb_orig', '1', '-6', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'devel_node_access', '0', 'palmweb_orig', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'devel', '0', 'palmweb_orig', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'devel', '1', 'palmweb_orig', '1', '9', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'devel', '2', 'palmweb_orig', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'block', '1', 'palmweb', '1', '0', 'left', '0', '0', '0', '', ''
    );
    db_query("INSERT INTO {boxes} (bid, body, info, format) VALUES (%d, '%s', '%s', '%s')",
        '1',
        '<img src="http://localhost/drupal-5.7/sites/default/themes/palmweb/coverimg.jpg"/>',
        'quick image block',
        '3'
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
        'cdm_dataportal', '2', 'palmweb', '1', '-7', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '3', 'palmweb', '0', '0', '', '0', '0', '0', '', ''
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

/************************************************************
*                       EXPORTING NODES                     *
************************************************************/

    system_initialize_theme_blocks('palmweb');

    return;
}

function palmweb_profile_install_menu($pid, $menu) {
    foreach ( $menu as $item) {
        $mid = db_next_id('{menu}_mid');
        db_query("INSERT INTO {menu} (mid, pid, path, title, description, weight, type) VALUES (%d,%d,'%s','%s','%s', %d, %d)",
                 $mid, $pid, $item['path'], $item['title'], $item['description'], $item['weight'], $item['type']);
        palmweb_profile_install_menu($mid, $item['children']);
    }
}

?>
<div class="dev-timer"> Page execution time was <em>569.38</em> ms. </div>