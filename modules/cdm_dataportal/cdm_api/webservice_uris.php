<?php

define('CDM_WS_PORTAL_MEDIA', 'portal/media');

define('CDM_WS_MEDIA_METADATA', 'media/$0/metadata');

define('CDM_WS_NAME', 'name');

define('CDM_WS_NAME_TYPEDESIGNATIONS', 'name/$0/typeDesignations');

define('CDM_WS_PORTAL_TAXON_NAMETYPEDESIGNATIONS', 'portal/taxon/$0/nameTypeDesignations');

define('CDM_WS_PORTAL_TAXON_DESCRIPTIONS', 'portal/taxon/$0/descriptions');

define('CDM_WS_PORTAL_NAME_DESCRIPTIONS', 'portal/name/$0/descriptions');

define('CDM_WS_REFERENCE', 'reference');

define('CDM_WS_NOMENCLATURAL_REFERENCE_CITATION', 'reference/$0/nomenclaturalCitation');

define('CDM_WS_PORTAL_TAXON_FIND', 'portal/taxon/find');

define('CDM_WS_PORTAL_TAXON', 'portal/taxon');

define('CDM_WS_PORTAL_TAXON', 'portal/taxon');

define('CDM_WS_NAME_NAMECAHE', 'name/$0/nameCache');

define('CDM_WS_PORTAL_TAXON_SYNONYMY', 'portal/taxon/$0/synonymy');

define('CDM_WS_PORTAL_TAXON_RELATIONS', 'portal/taxon/$0/taxonRelationships');

define('CDM_WS_PORTAL_TAXON_NAMERELATIONS', 'portal/taxon/$0/nameRelationships');


/**
 * @parameters $0 : the taxon uuid
 *
 * @returns the taxon which is the accepted synonym for the taxon given as
 * parameter taxonUuid. If the taxon specified by taxonUuid is itself the
 * accepted taxon, this one will be returned.
 */
define('CDM_WS_PORTAL_TAXON_ACCEPTED', 'portal/taxon/$0/accepted');

define('CDM_WS_PORTAL_TAXON_MEDIA', 'portal/taxon/$0/media/$1/$2');
define('CDM_WS_PORTAL_TAXON_SUBTREE_MEDIA', 'portal/taxon/$0/subtree/media/$1/$2');

/**
 *
 * Gets the root nodes of the taxonomic concept tree for the concept
 * reference specified by the secUuid parameter.
 *
 * stub: treenode_root
 */
define('CDM_WS_PORTAL_TAXONOMY',  'portal/taxontree');

define('CDM_WS_PORTAL_TAXONOMY_MEDIA', 'portal/taxontree/$0/$1');

define('CDM_WS_TERMVOCABULARY', 'term/$0');

define('CDM_WS_TERM_COMPARE', 'term/$0/compareTo/$1');

define('CDM_WS_TDWG_LEVEL', 'term/tdwg/$0');

/**
 * returns FeatureTrees that are stored in this community store
 *
 */
define('CDM_WS_FEATURETREE', 'featuretree/$0');

define('CDM_WS_FEATURETREES', 'featuretree');

define('CDM_WS_GEOSERVICE_DISTRIBUTIONMAP', 'geo/map/distribution/$0');
