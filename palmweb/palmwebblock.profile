<?php

    db_query(
        "DELETE FROM blocks WHERE theme = 'palmweb' 
    );

    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'block', '1', 'palmweb', '1', '-1', 'left', '0', '0', '0', '', ''
    );
    db_query("INSERT INTO {boxes} (bid, body, info, format) VALUES (%d, '%s', '%s', '%s')",
        '1',
        '<img src="http://localhost/drupal-5.7/sites/default/themes/palmweb/coverimg.jpg"/>',
        'palmweb block',
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
        'user', '0', 'palmweb', '1', '0', 'left', '0', '0', '0', '', 'User Login'
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '1', 'palmweb', '1', '0', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '2', 'palmweb', '0', '0', '', '0', '0', '1', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'user', '3', 'palmweb', '0', '0', '', '0', '0', '1', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '0', 'palmweb', '1', '1', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '1', 'palmweb', '1', '1', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '2', 'palmweb', '1', '0', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '3', 'palmweb', '0', '0', '', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_taxontree', '0', 'palmweb', '1', '-1', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'tinytax', 'tinytax_cdm_block', 'palmweb', '1', '0', 'right', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'devel_node_access', '0', 'palmweb', '0', '-10', '', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'devel', '0', 'palmweb', '0', '0', '', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'devel', '1', 'palmweb', '1', '1', 'left', '0', '0', '0', '', ''
    );
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'devel', '2', 'palmweb', '1', '1', 'left', '0', '0', '0', '', ''
    );
    
?>