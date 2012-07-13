# directory relative to build.xml that contains the gz files (sql database dumps)
gzdir = .

#database types
db_drupal = drupal
db_cdm = cdm

#prefix used in sql dump files
file_prefix_cdm = cdm_production_
file_prefix_drupal = drupal_dataportal_production_

# prefix to use for mysql databases
db_prefix_cdm = cdm_
db_prefix_drupal = drupal_dataportal_

#mysql user with administration rights
db.username = my_username
db.password = my_password

#@TODO: replace admin users with admin user for development
#example:
#UPDATE `drupal_dataportal_cichorieae`.`cdm_cichorieae_users` SET `name` = 'admin',
#`pass` = MD5( 'admin' ) ,
#`mail` = 'waddink@eti.uva.nl',
#`init` = 'waddink@eti.uva.nl' WHERE `cdm_cichorieae_users`.`uid` =1;