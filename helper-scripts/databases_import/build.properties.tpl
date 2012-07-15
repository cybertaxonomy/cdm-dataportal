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

#easy to remember name and password to use in temporary admin account for development usage only
admin.name = admin
admin.password = admin
#your email to use in admin account
admin.email = your_email@your_domain

#databases (without db_prefix_drupal) and table prefixes
# multiline properties seem not yet supported in older phing versions, so here on one line
db_array = cichorieae|cdm_cichorieae_,cyprus|cdm_cyprus_,flora_malesiana|cdm_flora_malesiana_,flore_afrique_centrale|flore_afrique_centrale_,flore_gabon|flore_gabon_,palmae|cdm_palm_