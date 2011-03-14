<?php
// vim: filetype=php

require ("../CDM_DataPortal/CDM_DataPortal.profile);

/************************************************************
*                           BLOCKS                          *
************************************************************/
    // Taxon tree
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_taxontree', 'cdm_tree', 'garland_EDIT', '1', '-9', 'left', '0', '0', '0', '', ''
    );
    // Search Taxa
    db_query(
        "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
        VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
        'cdm_dataportal', '2', 'garland', '1', '-10', 'left', '0', '0', '0', '', ''
    );

?>
