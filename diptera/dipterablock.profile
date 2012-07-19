<?php

    db_query(
        "DELETE FROM blocks WHERE theme = 'diptera' 
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
    );
    db_query(
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

?>
