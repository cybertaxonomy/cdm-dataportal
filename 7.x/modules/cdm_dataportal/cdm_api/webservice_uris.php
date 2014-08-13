<?php
/**
 * @file
 * CDM Server URI definitions.
 */

define('CDM_WS_REFERENCE', 'reference');
define('CDM_WS_REFERENCE_AUTHORTEAM', 'reference/$0/authorTeam');
define('CDM_WS_NOMENCLATURAL_REFERENCE_CITATION', 'reference/$0/nomenclaturalCitation');

define('CDM_WS_NAME', 'name');
define('CDM_WS_NAME_NAMECAHE', 'name/$0/nameCache');
define('CDM_WS_NAME_TYPEDESIGNATIONS', 'name/$0/typeDesignations');

define('CDM_WS_PORTAL_NAME_FINDBYNAME', 'name/findByName/');
define('CDM_WS_PORTAL_NAME_DESCRIPTIONS', 'portal/name/$0/taxonNameDescriptions');
define('CDM_WS_PORTAL_NAME_TYPEDESIGNATIONS', 'portal/name/$0/typeDesignations');

define('CDM_WS_TAXON', 'taxon');
define('CDM_WS_TAXON_CLASSIFICATIONS', 'taxon/$0/classifications');
define('CDM_WS_TAXON_FINDBY_DESCRIPTIONELEMENT_FULLTEXT', 'taxon/findByDescriptionElementFullText');
define('CDM_WS_TAXON_FINDBESTMATCHINGTAXON', '/taxon/bestMatchingTaxon/$0');
/**
 * Parameter $0 : the taxon uuid.
 *
 * Returns the taxon which is the accepted synonym for the taxon given as
 * parameter taxonUuid. If the taxon specified by taxonUuid is itself the
 * accepted taxon, this one will be returned.
 */
define('CDM_WS_PORTAL_TAXON_ACCEPTED', 'portal/taxon/$0/accepted/$1');

define('CDM_WS_PORTAL_TAXON', 'portal/taxon');
define('CDM_WS_PORTAL_TAXON_SYNONYMY', 'portal/taxon/$0/synonymy');
define('CDM_WS_PORTAL_TAXON_RELATIONS', 'portal/taxon/$0/taxonRelationships');
define('CDM_WS_PORTAL_TAXON_DESCRIPTIONS', 'portal/taxon/$0/descriptions');

/**
 * Parameter $0: taxon UUID.
 * Parameter $1: simple name of the class extending DescriptionElementBase.
 */
define('CDM_WS_PORTAL_TAXON_DESCRIPTIONS_ELEMENTSBYTYPE', 'portal/taxon/$0/descriptions/elementsByType/$1');
define('CDM_WS_PORTAL_TAXON_USEDESCRIPTIONS', 'portal/taxon/$0/useDescriptions');
define('CDM_WS_PORTAL_TAXON_TO_NAMERELATIONS', 'portal/taxon/$0/toNameRelationships');
define('CDM_WS_PORTAL_TAXON_FROM_NAMERELATIONS', 'portal/taxon/$0/fromNameRelationships');
define('CDM_WS_PORTAL_TAXON_MEDIA', 'portal/taxon/$0/media');
define('CDM_WS_PORTAL_TAXON_SUBTREE_MEDIA', 'portal/taxon/$0/subtree/media');

define('CDM_WS_PORTAL_TAXON_TAXONNODES', 'portal/taxon/$0/taxonNodes');
define('CDM_WS_PORTAL_TAXON_FIND', 'portal/taxon/find');
define('CDM_WS_PORTAL_TAXON_SEARCH', 'portal/taxon/search');
define('CDM_WS_PORTAL_TAXON_FINDBY_DESCRIPTIONELEMENT_FULLTEXT', 'portal/taxon/findByDescriptionElementFullText');

/**
 * /description/{uuid}/naturalLanguageDescription/{featuretree_uuid}
 */
define('CDM_WS_DESCRIPTION_NAMEDAREAS_IN_USE', 'description/namedAreasInUse');
define('CDM_WS_DESCRIPTION_NATURALLANGUAGE_DESCRIPTION', 'description/$0/naturalLanguageDescription/$1');
define('CDM_WS_DESCRIPTION_HAS_STRUCTRURED_DATA', 'description/$0/hasStructuredData');

define('CDM_WS_PORTAL_DESCRIPTION', 'portal/description/$0');
define('CDM_WS_PORTAL_DESCRIPTION_AREAS_TREE', 'portal/description/$0/namedAreaTree');
/**
 * @deprecated use CDM_WS_PORTAL_DESCRIPTION_DISTRIBUTION_INFO_FOR instead
 */
define('CDM_WS_PORTAL_DESCRIPTION_DISTRIBUTION_TREE', 'portal/description/$0/DistributionTree');
define('CDM_WS_PORTAL_DESCRIPTION_DISTRIBUTION_INFO_FOR', 'portal/description/distributionInfoFor/$0');

define('CDM_WS_DESCRIPTIONELEMENT', 'descriptionElement/$0');
define('CDM_WS_DESCRIPTIONELEMENT_BY_TAXON', 'portal/descriptionElement/byTaxon');
define('CDM_WS_PORTAL_DESCRIPTIONELEMENT', 'portal/descriptionElement/$0');

/**
 * Gets the root nodes of the taxonomic concept tree for the concept
 * reference specified by the secUuid parameter.
 *
 * stub: treenode_root
 */
define('CDM_WS_PORTAL_TAXONOMY', 'portal/classification');
define('CDM_WS_PORTAL_TAXONOMY_CHILDNODES', 'portal/classification/$0/childNodes');
define('CDM_WS_PORTAL_TAXONOMY_CHILDNODES_AT_RANK', 'portal/classification/$0/childNodesAt/$1');
define('CDM_WS_PORTAL_TAXONOMY_CHILDNODES_OF_TAXON', 'portal/classification/$0/childNodesOf/$1');
define('CDM_WS_PORTAL_TAXONOMY_PATH_FROM', 'portal/classification/$0/pathFrom/$1');
define('CDM_WS_PORTAL_TAXONOMY_PATH_FROM_TO_RANK', 'portal/classification/$0/pathFrom/$1/toRank/$2');
define('CDM_WS_PORTAL_TAXONOMY_MEDIA', 'portal/classification/$0/$1');

define('CDM_WS_TERMVOCABULARY', 'termVocabulary/$0');

define('CDM_WS_TERM_COMPARE', 'term/$0/compareTo/$1');

/**
 * Returns FeatureTrees that are stored in this community store.
 */
define('CDM_WS_FEATURETREE', 'portal/featureTree/$0');
define('CDM_WS_FEATURETREES', 'portal/featureTree');
// define('CDM_WS_FEATURETREE_CHILDREN', 'featureTree/$0/children');
define('CDM_WS_GEOSERVICE_DISTRIBUTIONMAP', 'ext/edit/mapServiceParameters/taxonDistributionFor/$0');
define('CDM_WS_GEOSERVICE_OCCURRENCEMAP', 'ext/edit/mapServiceParameters/taxonOccurrencesFor/$0');

define('CDM_WS_OCCURRENCE', 'occurrence');
define('CDM_WS_PORTAL_OCCURRENCE', 'portal/occurrence');
/**
 * url query paramaters:
 *
 * @param taxonUuid
 * @param relationshipUuids e.g. CongruentTo;  "60974c98-64ab-4574-bb5c-c110f6db634d"
 * @param relationshipInversUuids
 * @param maxDepth null for unlimited
 * @param pageNumber
 * @param pageSize
 * @var unknown_type
 */
define('CDM_WS_OCCURRENCE_BY_ASSOCIATEDTAXON', 'occurrence/byAssociatedTaxon');

define('CDM_WS_DERIVEDUNIT_FACADE', 'derivedUnitFacade/$0');

define('CDM_WS_IDENTIFICATIONKEY', 'identificationKey');
define('CDM_WS_MEDIAKEY', 'mediaKey');
define('CDM_WS_MULTIACCESSKEY', 'multiAccessKey');
define('CDM_WS_POLYTOMOUSKEY', 'polytomousKey');
define('CDM_WS_POLYTOMOUSKEY_NODE', 'polytomousKeyNode');

define('CDM_WS_PORTAL_MEDIA', 'portal/media');

define('CDM_WS_MEDIA_METADATA', 'media/$0/metadata');

define('CDM_WS_MANAGE_REINDEX', 'manage/reindex');
define('CDM_WS_MANAGE_PURGE', 'manage/purge');
