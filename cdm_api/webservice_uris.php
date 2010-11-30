<?php

define('CDM_WS_PORTAL_MEDIA', 'portal/media');

define('CDM_WS_MEDIA_METADATA', 'media/$0/metadata');

define('CDM_WS_NAME', 'name');

define('CDM_WS_TAXON', 'taxon');

define('CDM_WS_NAME_TYPEDESIGNATIONS', 'name/$0/typeDesignations');

define('CDM_WS_PORTAL_NAME_TYPEDESIGNATIONS', 'portal/name/$0/typeDesignations');

define('CDM_WS_PORTAL_TAXON_NAMETYPEDESIGNATIONS', 'portal/taxon/$0/nameTypeDesignations');

define('CDM_WS_PORTAL_TAXON_DESCRIPTIONS', 'portal/taxon/$0/descriptions');

/**
 * $0 : taxon uud
 * $1 : simple name of class extending DescriptionElementBase
 *
 */
define('CDM_WS_PORTAL_TAXON_DESCRIPTIONS_ELEMENTSBYTYPE', 'portal/taxon/$0/descriptions/elementsByType/$1');

define('CDM_WS_PORTAL_NAME_DESCRIPTIONS', 'portal/name/$0/taxonNameDescriptions');

define('CDM_WS_PORTAL_DESCRIPTION_AREAS_TREE', 'portal/description/$0/namedAreaTree');
define('CDM_WS_PORTAL_DESCRIPTION_DISTRIBUTION_TREE', 'portal/description/$0/DistributionTree');

define('CDM_WS_REFERENCE', 'reference');

define('CDM_WS_NOMENCLATURAL_REFERENCE_CITATION', 'reference/$0/nomenclaturalCitation');

define('CDM_WS_REFERENCE_AUTHORTEAM', 'reference/$0/authorTeam');

define('CDM_WS_PORTAL_TAXON_FIND', 'portal/taxon/find');

define('CDM_WS_PORTAL_TAXON', 'portal/taxon');

define('CDM_WS_PORTAL_TAXON_TAXONNODES', 'portal/taxon/$0/taxonNodes');

define('CDM_WS_NAME_NAMECAHE', 'name/$0/nameCache');

define('CDM_WS_PORTAL_TAXON_SYNONYMY', 'portal/taxon/$0/synonymy');

define('CDM_WS_PORTAL_TAXON_RELATIONS', 'portal/taxon/$0/taxonRelationships');

define('CDM_WS_PORTAL_TAXON_NAMERELATIONS', 'portal/taxon/$0/nameRelationships');

define('CDM_WS_PORTAL_TAXON_TO_NAMERELATIONS', 'portal/taxon/$0/toNameRelationships');

define('CDM_WS_PORTAL_TAXON_FROM_NAMERELATIONS', 'portal/taxon/$0/fromNameRelationships');

define('CDM_WS_DESCRIPTION_HAS_STRUCTRURED_DATA', 'description/$0/hasStructuredData');

/**
 * /description/{uuid}/naturalLanguageDescription/{featuretree_uuid}
 */
define('CDM_WS_DESCRIPTION_NATURALLANGUAGE_DESCRIPTION', 'description/$0/naturalLanguageDescription/$1');

define('CDM_WS_DESCRIPTIONELEMENT', 'descriptionElement/$0');

define('CDM_WS_PORTAL_DESCRIPTIONELEMENT', 'portal/descriptionElement/$0');


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
define('CDM_WS_PORTAL_TAXONOMY',  'portal/classification');

define('CDM_WS_PORTAL_TAXONOMY_CHILDNODES',  'portal/classification/$0/childNodes');

define('CDM_WS_PORTAL_TAXONOMY_CHILDNODES_AT_RANK',  'portal/classification/$0/childNodesAt/$1');

define('CDM_WS_PORTAL_TAXONOMY_CHILDNODES_OF_TAXON',  'portal/classification/$0/childNodesOf/$1');

define('CDM_WS_PORTAL_TAXONOMY_PATH_FROM',  'portal/classification/$0/pathFrom/$1');

define('CDM_WS_PORTAL_TAXONOMY_PATH_FROM_TO_RANK',  'portal/classification/$0/pathFrom/$1/toRank/$2');


define('CDM_WS_PORTAL_TAXONOMY_MEDIA', 'portal/classification/$0/$1');

define('CDM_WS_TERMVOCABULARY', 'termVocabulary/$0');

define('CDM_WS_TERM_COMPARE', 'term/$0/compareTo/$1');

define('CDM_WS_TDWG_LEVEL', 'term/tdwg/$0');

/**
 * returns FeatureTrees that are stored in this community store
 *
 */
define('CDM_WS_FEATURETREE', 'featureTree/$0');

//define('CDM_WS_FEATURETREE_CHILDREN', 'featuretree/$0/children');

define('CDM_WS_FEATURETREES', 'featureTree');

define('CDM_WS_GEOSERVICE_DISTRIBUTIONMAP', 'ext/edit/mapServiceParameters/taxonDistributionFor/$0');
define('CDM_WS_GEOSERVICE_OCCURRENCEMAP', 'ext/edit/mapServiceParameters/taxonOccurrencesFor/$0');

define('CDM_WS_DERIVEDUNIT_FACADE', 'derivedUnitFacade/$0');
