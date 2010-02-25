<?php

// vim: filetype=php

/************************************************************
*                           MODULES                         *
************************************************************/
function generated_profile_modules() {
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
function generated_profile_details() {
    return array (
        'name' => 'Generated',
        'description' => 'Installation profile generated automatically on 2nd May 2008 03:55pm',
      );
}

function generated_profile_final() {
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
    variable_set('cdm_webservice_cache', 1);
    variable_set('cdm_webservice_debug', 0);
    variable_set('cdm_webservice_isStub', 1);
    variable_set('cdm_webservice_proxy_port', '8080');
    variable_set('cdm_webservice_proxy_pwd', '');
    variable_set('cdm_webservice_proxy_url', 'proxy.ad.kew.org');
    variable_set('cdm_webservice_proxy_usr', '');
    variable_set('cdm_webservice_type', 'json');
    variable_set('cdm_webservice_url', 'http://dev.e-taxonomy.eu/svn/trunk/drupal/modules/cdm_dataportal/cdm_api/ws_stub/');
    variable_set('color_garland_files', array (
      0 => 'files/color/garland-e6c48917/menu-collapsed.gif',
      1 => 'files/color/garland-e6c48917/menu-expanded.gif',
      2 => 'files/color/garland-e6c48917/menu-leaf.gif',
      3 => 'files/color/garland-e6c48917/body.png',
      4 => 'files/color/garland-e6c48917/bg-bar.png',
      5 => 'files/color/garland-e6c48917/bg-bar-white.png',
      6 => 'files/color/garland-e6c48917/bg-tab.png',
      7 => 'files/color/garland-e6c48917/bg-navigation.png',
      8 => 'files/color/garland-e6c48917/bg-content-left.png',
      9 => 'files/color/garland-e6c48917/bg-content-right.png',
      10 => 'files/color/garland-e6c48917/bg-content.png',
      11 => 'files/color/garland-e6c48917/bg-navigation-item.png',
      12 => 'files/color/garland-e6c48917/bg-navigation-item-hover.png',
      13 => 'files/color/garland-e6c48917/gradient-inner.png',
      14 => 'files/color/garland-e6c48917/logo.png',
      15 => 'files/color/garland-e6c48917/screenshot.png',
      16 => 'files/color/garland-e6c48917/style.css',
    ));
    variable_set('color_garland_logo', 'files/color/garland-e6c48917/logo.png');
    variable_set('color_garland_palette', array (
      'base' => '#8fc3f0',
      'link' => '#027ac6',
      'top' => '#2385c2',
      'bottom' => '#5ab5ee',
      'text' => '#494949',
    ));
    variable_set('color_garland_screenshot', 'files/color/garland-e6c48917/screenshot.png');
    variable_set('color_garland_stylesheet', 'files/color/garland-e6c48917/style.css');

    variable_set('color_diptera_files', array (
      0 => 'files/color/diptera-97c90566/menu-collapsed.gif',
      1 => 'files/color/diptera-97c90566/menu-expanded.gif',
      2 => 'files/color/diptera-97c90566/menu-leaf.gif',
      3 => 'files/color/diptera-97c90566/body.png',
      4 => 'files/color/diptera-97c90566/bg-bar.png',
      5 => 'files/color/diptera-97c90566/bg-bar-white.png',
      6 => 'files/color/diptera-97c90566/bg-tab.png',
      7 => 'files/color/diptera-97c90566/bg-navigation.png',
      8 => 'files/color/diptera-97c90566/bg-content-left.png',
      9 => 'files/color/diptera-97c90566/bg-content-right.png',
      10 => 'files/color/diptera-97c90566/bg-content.png',
      11 => 'files/color/diptera-97c90566/bg-navigation-item.png',
      12 => 'files/color/diptera-97c90566/bg-navigation-item-hover.png',
      13 => 'files/color/diptera-97c90566/gradient-inner.png',
      14 => 'files/color/diptera-97c90566/logo.png',
      15 => 'files/color/diptera-97c90566/screenshot.png',
      16 => 'files/color/diptera-97c90566/style.css',
    ));
    variable_set('color_diptera_logo', 'files/color/diptera-97c90566/logo.png');
    variable_set('color_diptera_palette', array (
      'base' => '#ffe23d',
      'link' => '#a9290a',
      'top' => '#fc6d1d',
      'bottom' => '#a30f42',
      'text' => '#494949',
    ));
    variable_set('color_diptera_screenshot', 'files/color/diptera-97c90566/screenshot.png');
    variable_set('color_diptera_stylesheet', 'files/color/diptera-97c90566/style.css');

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
    variable_set('file_directory_temp', '');
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
    variable_set('site_mail', '');
    variable_set('site_mission', '');
    variable_set('site_name', 'CDM dataportal');
    variable_set('site_offline', '0');
    variable_set('site_offline_message', 'The CDM dataportal is currently offline for maintenance. Please check back later.');
    variable_set('site_slogan', '');
    variable_set('smtp_library', '');

    system_theme_data();
    db_query("UPDATE {system} SET status = 1 WHERE type = 'theme' and name = '%s'", 'diptera');
    variable_set('theme_default', 'diptera');
    variable_set('theme_diptera_settings', array (
      'toggle_logo' => 1,
      'toggle_name' => 0,
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
      'form_token' => 'f8e4c600b8d0e5f7b5a2eabe6fe3b021',
    ));

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
      'form_token' => '0790ae766d51bcca95f18a36162b0afc',
      'scheme' => '',
      'palette' => 
      array (
        'base' => '#8fc3f0',
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
    $rid = db_next_id('{role}_rid');
    $role_id['developer'] = $rid;
    db_query("INSERT INTO {role} (rid, name) VALUES (%d, '%s')", $rid, 'developer');
    db_query("INSERT INTO {permission} (rid, perm) VALUES (%d, '%s')", $rid, 'access administration menu, display drupal links, administer blocks, use PHP for block visibility, administer cdm_dataportal, cdm_dataportal view notes, access comments, administer comments, post comments, post comments without approval, access devel information, execute php code, switch users, view devel_node_access information, administer filters, administer menu, access content, administer content types, administer nodes, create page content, create story content, edit own page content, edit own story content, edit page content, edit story content, revert revisions, view revisions, profile_generator generate, access administration pages, administer site configuration, select different theme, administer taxonomy, access user profiles, administer access control, administer users, change own username');
    $rid = db_next_id('{role}_rid');
    $role_id['administrator'] = $rid;
    db_query("INSERT INTO {role} (rid, name) VALUES (%d, '%s')", $rid, 'administrator');
    db_query("INSERT INTO {permission} (rid, perm) VALUES (%d, '%s')", $rid, 'display drupal links, administer blocks, cdm_dataportal view notes, access comments, administer comments, post comments, post comments without approval, administer menu, access content, administer content types, administer nodes, create page content, create story content, edit own page content, edit own story content, edit page content, edit story content, revert revisions, view revisions, administer site configuration, select different theme, administer taxonomy, access user profiles, administer users');

/************************************************************
*                            USERS                          *
************************************************************/
    $user = user_save(new stdClass(), array (
        'name' => 'develtest',
        'mail' => 'd.taylor@kew.org',
        'mode' => '0',
        'sort' => '0',
        'threshold' => '0',
        'theme' => '',
        'signature' => '',
        'created' => '1203348986',
        'access' => '1209740113',
        'login' => '1209738756',
        'status' => '1',
        'timezone' => '0',
        'language' => '',
        'picture' => '',
        'init' => 'd.taylor@kew.org',
        'data' => 'a:0:{}',
      ));
    db_query("UPDATE {users} SET pass='%s' WHERE uid=%d", '32f5be2d52c73d367e4e4c49ec55af3f', $user->uid);
    $user_id['develtest'] = $user->uid;
    $user = user_save(new stdClass(), array (
        'name' => 'admintest',
        'mail' => 'd.taylor@kew.org.uk',
        'mode' => '0',
        'sort' => '0',
        'threshold' => '0',
        'theme' => '',
        'signature' => '',
        'created' => '1209731659',
        'access' => '1209738742',
        'login' => '1209738741',
        'status' => '1',
        'timezone' => '0',
        'language' => '',
        'picture' => '',
        'init' => 'd.taylor@kew.org.uk',
        'data' => 'a:0:{}',
      ));
    db_query("UPDATE {users} SET pass='%s' WHERE uid=%d", '66d4aaa5ea177ac32c69946de3731ec0', $user->uid);
    $user_id['admintest'] = $user->uid;

/************************************************************
*                   USERS <=> ROLES MAPPING                 *
************************************************************/
    db_query("INSERT INTO {users_roles} (uid, rid) VALUES (%d, %d)", $user_id['admintest'], $role_id['administrator']);
    db_query("INSERT INTO {users_roles} (uid, rid) VALUES (%d, %d)", $user_id['develtest'], $role_id['developer']);

/************************************************************
*                            MENUS                          *
************************************************************/

    while ( db_next_id('{menu}_mid') < 2) {}

    // first the primary links
    generated_profile_install_menu(2, array (
      0 => 
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
    generated_profile_install_menu(0, array (
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
        'block', '1', 'chameleon', '1', '0', 'left', '0', '0', '0', '', ''
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
        'user', '0', 'chameleon', '1', '0', 'left', '0', '0', '0', '', 'User Login'
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
        'cdm_dataportal', '0', 'chameleon', '1', '-5', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '1', 'chameleon', '1', '0', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '2', 'chameleon', '1', '-7', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '3', 'chameleon', '1', '0', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_taxontree', '0', 'chameleon', '1', '0', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'tinytax', 'tinytax_cdm_block', 'chameleon', '1', '0', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'devel_node_access', '0', 'chameleon', '1', '0', 'left', '0', '0', '0', '', ''
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
        'devel', '2', 'chameleon', '1', '0', 'left', '0', '0', '0', '', ''
    );
    
    
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'block', '1', 'garland', '1', '0', 'left', '0', '0', '0', '', ''
    );

    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'comment', '0', 'garland', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'menu', $menu['2'], 'garland', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'node', '0', 'garland', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '0', 'garland', '1', '0', 'left', '0', '0', '0', '', 'User Login'
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '1', 'garland', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '2', 'garland', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '3', 'garland', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '0', 'garland', '1', '-5', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '1', 'garland', '1', '0', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '2', 'garland', '1', '-7', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '3', 'garland', '1', '0', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_taxontree', '0', 'garland', '1', '0', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'tinytax', 'tinytax_cdm_block', 'garland', '1', '0', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'devel_node_access', '0', 'garland', '1', '0', 'footer', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'devel', '0', 'garland', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'devel', '1', 'garland', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'devel', '2', 'garland', '1', '0', 'left', '0', '0', '0', '', ''
    );

    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'block', '1', 'bluemarine', '0', '0', 'left', '0', '0', '0', '', ''
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
        'user', '0', 'bluemarine', '0', '0', 'left', '0', '0', '0', '', 'User Login'
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
        'cdm_dataportal', '0', 'bluemarine', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '1', 'bluemarine', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '2', 'bluemarine', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '3', 'bluemarine', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_taxontree', '0', 'bluemarine', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'tinytax', 'tinytax_cdm_block', 'bluemarine', '0', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'devel_node_access', '0', 'bluemarine', '1', '0', 'footer', '0', '0', '0', '', ''
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
        'block', '2', 'diptera', '1', '-2', 'left', '0', '0', '0', '', ''
    );
    db_query("INSERT INTO {boxes} (bid, body, info, format) VALUES (%d, '%s', '%s', '%s')",
        '2',
        '<img src="http://localhost/drupal-5.7/sites/default/themes/diptera/coverimg.jpg"/>',
        'diptera block',
        '3'
    );+
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'block', '1', 'diptera', '0', '-2', '', '0', '0', '0', '', ''
    );

    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'comment', '0', 'diptera', '0', '0', '', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'menu', $menu['2'], 'diptera', '0', '0', '', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'node', '0', 'diptera', '0', '0', '', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '0', 'diptera', '1', '-1', 'left', '0', '0', '0', '', 'User Login'
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '1', 'diptera', '1', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '2', 'diptera', '0', '0', '', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '3', 'diptera', '0', '0', '', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '0', 'diptera', '1', '1', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '1', 'diptera', '1', '1', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '2', 'diptera', '1', '-1', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '3', 'diptera', '0', '0', '', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_taxontree', '0', 'diptera', '1', '-2', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'tinytax', 'tinytax_cdm_block', 'diptera', '1', '0', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'devel_node_access', '0', 'diptera', '0', '-10', '', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'devel', '0', 'diptera', '0', '0', '', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'devel', '1', 'diptera', '1', '10', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'devel', '2', 'diptera', '1', '1', 'left', '0', '0', '0', '', ''
    );

    db_query(
        "INSERT INTO {blocks_roles} (module,delta,rid) VALUES ('%s', '%s', %d)",
        'block', '1', $role_id['anonymous user']
    );
    db_query(
        "INSERT INTO {blocks_roles} (module,delta,rid) VALUES ('%s', '%s', %d)",
        'block', '1', $role_id['authenticated user']
    );
    db_query(
        "INSERT INTO {blocks_roles} (module,delta,rid) VALUES ('%s', '%s', %d)",
        'block', '1', $role_id['developer']
    );
    db_query(
        "INSERT INTO {blocks_roles} (module,delta,rid) VALUES ('%s', '%s', %d)",
        'block', '1', $role_id['administrator']
    );
    db_query(
        "INSERT INTO {blocks_roles} (module,delta,rid) VALUES ('%s', '%s', %d)",
        'cdm_dataportal', '0', $role_id['developer']
    );
    db_query(
        "INSERT INTO {blocks_roles} (module,delta,rid) VALUES ('%s', '%s', %d)",
        'cdm_dataportal', '1', $role_id['anonymous user']
    );
    db_query(
        "INSERT INTO {blocks_roles} (module,delta,rid) VALUES ('%s', '%s', %d)",
        'cdm_dataportal', '1', $role_id['authenticated user']
    );
    db_query(
        "INSERT INTO {blocks_roles} (module,delta,rid) VALUES ('%s', '%s', %d)",
        'cdm_dataportal', '1', $role_id['developer']
    );
    db_query(
        "INSERT INTO {blocks_roles} (module,delta,rid) VALUES ('%s', '%s', %d)",
        'cdm_dataportal', '1', $role_id['administrator']
    );
    db_query(
        "INSERT INTO {blocks_roles} (module,delta,rid) VALUES ('%s', '%s', %d)",
        'cdm_dataportal', '2', $role_id['anonymous user']
    );
    db_query(
        "INSERT INTO {blocks_roles} (module,delta,rid) VALUES ('%s', '%s', %d)",
        'cdm_dataportal', '2', $role_id['authenticated user']
    );
    db_query(
        "INSERT INTO {blocks_roles} (module,delta,rid) VALUES ('%s', '%s', %d)",
        'cdm_dataportal', '2', $role_id['developer']
    );
    db_query(
        "INSERT INTO {blocks_roles} (module,delta,rid) VALUES ('%s', '%s', %d)",
        'cdm_dataportal', '2', $role_id['administrator']
    );
    db_query(
        "INSERT INTO {blocks_roles} (module,delta,rid) VALUES ('%s', '%s', %d)",
        'cdm_taxontree', '0', $role_id['anonymous user']
    );
    db_query(
        "INSERT INTO {blocks_roles} (module,delta,rid) VALUES ('%s', '%s', %d)",
        'cdm_taxontree', '0', $role_id['authenticated user']
    );
    db_query(
        "INSERT INTO {blocks_roles} (module,delta,rid) VALUES ('%s', '%s', %d)",
        'cdm_taxontree', '0', $role_id['developer']
    );
    db_query(
        "INSERT INTO {blocks_roles} (module,delta,rid) VALUES ('%s', '%s', %d)",
        'cdm_taxontree', '0', $role_id['administrator']
    );
    db_query(
        "INSERT INTO {blocks_roles} (module,delta,rid) VALUES ('%s', '%s', %d)",
        'devel', '2', $role_id['developer']
    );
    db_query(
        "INSERT INTO {blocks_roles} (module,delta,rid) VALUES ('%s', '%s', %d)",
        'tinytax', 'tinytax_cdm_block', $role_id['developer']
    );
    db_query(
        "INSERT INTO {blocks_roles} (module,delta,rid) VALUES ('%s', '%s', %d)",
        'user', '0', $role_id['anonymous user']
    );
    db_query(
        "INSERT INTO {blocks_roles} (module,delta,rid) VALUES ('%s', '%s', %d)",
        'user', '0', $role_id['authenticated user']
    );
    db_query(
        "INSERT INTO {blocks_roles} (module,delta,rid) VALUES ('%s', '%s', %d)",
        'user', '0', $role_id['developer']
    );
    db_query(
        "INSERT INTO {blocks_roles} (module,delta,rid) VALUES ('%s', '%s', %d)",
        'user', '0', $role_id['administrator']
    );
    db_query(
        "INSERT INTO {blocks_roles} (module,delta,rid) VALUES ('%s', '%s', %d)",
        'user', '1', $role_id['developer']
    );
    db_query(
        "INSERT INTO {blocks_roles} (module,delta,rid) VALUES ('%s', '%s', %d)",
        'user', '1', $role_id['administrator']
    );

/************************************************************
*                       EXPORTING NODES                     *
************************************************************/
    // exporting nodes of type: Page
    db_query(
        "INSERT INTO {node} (nid,vid,type,title,uid,status,created,changed,comment,promote,moderate,sticky)
        VALUES ('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",
        '1','1','page','Introduction','1','1','1203355397','1209496950','0','0','0','0'
    );
    db_query(
        "INSERT INTO {node_revisions} (nid,vid,uid,title,body,teaser,log,timestamp,format)
        VALUES ('%s','%s','%s','%s','%s','%s','%s','%s','%s')",
        '1','1','1','Introduction','Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum
    
    <ul>
    <li>Lorem ipsum dolor sit amet
    <li>consectetur adipisicing elit
    <li>sed do eiusmod tempor incididunt ut labore et dolore magna aliqu
    </ul>
    
    Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum
    
    Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum','Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum
    
    <ul>
    <li>Lorem ipsum dolor sit amet
    <li>consectetur adipisicing elit
    <li>sed do eiusmod tempor incididunt ut labore et dolore magna aliqu
    </ul>
    
','','1209496950','3'
    );
    // exporting nodes of type: Story

    return;
}

function generated_profile_install_menu($pid, $menu) {
    foreach ( $menu as $item) {
        $mid = db_next_id('{menu}_mid');
        db_query("INSERT INTO {menu} (mid, pid, path, title, description, weight, type) VALUES (%d,%d,'%s','%s','%s', %d, %d)",
                 $mid, $pid, $item['path'], $item['title'], $item['description'], $item['weight'], $item['type']);
        generated_profile_install_menu($mid, $item['children']);
    }
}

?>
<div class="dev-timer"> Page execution time was <em>659.99</em> ms. </div>