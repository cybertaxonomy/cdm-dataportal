<?php
/**
 * @file
 * CDM Server URI definitions.
 */

define('CDM_WS_PORTAL_GENERIC', 'portal/$0');

const CDM_WS_CLASSIFICATION = 'classification';

define('CDM_WS_AGENT', 'agent');
define('CDM_WS_PORTAL_AGENT', 'portal/agent');
define('CDM_WS_REFERENCE', 'reference');
define('CDM_WS_REFERENCE_AUTHORTEAM', 'reference/$0/authorship');
/**
 * Parameter $0 : the reference uuid
 *
 * Provides the protologue and original publication links of the name
 */
define('CDM_WS_NAME_PROTOLOGUE_LINKS', 'name/$0/protologueLinks');
define('CDM_WS_NOMENCLATURAL_REFERENCE_CITATION', 'reference/$0/nomenclaturalCitation');

define('CDM_WS_NAME', 'name');
define('CDM_WS_NAME_NAMECAHE', 'name/$0/nameCache');
define('CDM_WS_NAME_TYPEDESIGNATIONS', 'name/$0/typeDesignations');
define('CDM_WS_TYPE_DESIGNATION_STATUS_FILTER_TERMS', 'name/typeDesignationStatusFilterTerms');

define('CDM_WS_PORTAL_NAME', 'portal/name');
define('CDM_WS_PORTAL_NAME_FINDBYNAME', 'name/findByName/');
define('CDM_WS_PORTAL_NAME_DESCRIPTIONS', 'portal/name/$0/taxonNameDescriptions');
define('CDM_WS_PORTAL_NAME_TYPEDESIGNATIONS', 'portal/name/$0/typeDesignations');
define('CDM_WS_PORTAL_NAME_TYPEDESIGNATIONS_IN_HOMOTYPICAL_GROUP', 'portal/name/$0/typeDesignationsInHomotypicalGroup');
define('CDM_WS_PORTAL_NAME_NAME_RELATIONS', 'portal/name/$0/nameRelations');

define('CDM_WS_NOMENCLATURALSTATUS', 'nomenclaturalStatus/$0');

define('CDM_WS_TYPEDESIGNATION', 'typedesignation/$0');
define('CDM_WS_PORTAL_TYPEDESIGNATION', 'portal/typedesignation/$0');


define('CDM_WS_TAXON', 'taxon');

define('CDM_WS_PORTAL_TAXON_ROOTUNIT_DTOS', 'portal/taxon/$0/rootUnitDTOs');

define('CDM_WS_TAXON_CLASSIFICATIONS', 'taxon/$0/classifications');
define('CDM_WS_TAXON_FINDBY_DESCRIPTIONELEMENT_FULLTEXT', 'taxon/findByDescriptionElementFullText');
/**
 * Parameter $0 : the taxon uuid.
 *
 * Returns the taxon which is the accepted synonym for the taxon given as
 * parameter taxonUuid. If the taxon specified by taxonUuid is itself the
 * accepted taxon, this one will be returned.
 */
define('CDM_WS_PORTAL_TAXON_ACCEPTED', 'portal/taxon/$0/accepted');

define('CDM_WS_PORTAL_TAXON', 'portal/taxon');
define('CDM_WS_PORTAL_TAXON_SYNONYMY', 'portal/taxon/$0/synonymy');
define('CDM_WS_PORTAL_TAXON_RELATIONS', 'portal/taxon/$0/taxonRelationships');
define('CDM_WS_PORTAL_TAXON_RELATIONS_DTO', 'taxon/$0/taxonRelationshipsDTO');
define('CDM_WS_PORTAL_TAXON_DESCRIPTIONS', 'portal/taxon/$0/descriptions');
define('CDM_WS_PORTAL_TAXON_TAXONNODEAGENTRELATIONS', 'portal/taxon/$0/taxonNodeAgentRelations/$1');

/**
 * Parameter $0: taxon UUID.
 * Parameter $1: simple name of the class extending DescriptionElementBase.
 */
define('CDM_WS_PORTAL_TAXON_DESCRIPTIONS_ELEMENTSBYTYPE', 'portal/taxon/$0/descriptions/elementsByType/$1');
define('CDM_WS_PORTAL_TAXON_USEDESCRIPTIONS', 'portal/taxon/$0/useDescriptions');
define('CDM_WS_PORTAL_TAXON_ASSOCIATED_ROOTUNITS', 'portal/taxon/$0/associatedRootUnits');
define('CDM_WS_PORTAL_TAXON_TO_NAMERELATIONS', 'portal/taxon/$0/toNameRelationships');
define('CDM_WS_PORTAL_TAXON_FROM_NAMERELATIONS', 'portal/taxon/$0/fromNameRelationships');
define('CDM_WS_PORTAL_TAXON_MEDIA', 'portal/taxon/$0/media');
define('CDM_WS_PORTAL_TAXON_SUBTREE_MEDIA', 'portal/taxon/$0/subtree/media');

define('CDM_WS_PORTAL_TAXON_TAXONNODES', 'portal/taxon/$0/taxonNodes');
define('CDM_WS_PORTAL_TAXON_FIND', 'portal/taxon/find');
define('CDM_WS_PORTAL_TAXON_SEARCH', 'portal/taxon/search');
define('CDM_WS_TAXON_SEARCH', 'taxon/search');
define('CDM_WS_PORTAL_TAXON_FINDBY_DESCRIPTIONELEMENT_FULLTEXT', 'portal/taxon/findByDescriptionElementFullText');

define('CDM_WS_IIIF_TAXON_MANIFEST', 'iiif/taxon/$0/manifest');


define('CDM_WS_TAXONNODE', 'taxonNode/$0');

/**
 * /description/{uuid}/naturalLanguageDescription/{termtree_uuid}
 */
define('CDM_WS_DESCRIPTION_NAMEDAREAS_IN_USE', 'description/namedAreasInUse');
define('CDM_WS_DESCRIPTION_NATURALLANGUAGE_DESCRIPTION', 'description/$0/naturalLanguageDescription/$1');
define('CDM_WS_DESCRIPTION_HAS_STRUCTRURED_DATA', 'description/$0/hasStructuredData');

define('CDM_WS_PORTAL_DESCRIPTION', 'portal/description/$0');
define('CDM_WS_PORTAL_DESCRIPTION_AREAS_TREE', 'portal/description/$0/namedAreaTree');


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
/**
 * TODO: harmonize return type, the REST service should return TaxonNodeDto, see #6222
 *
 * returns list of TaxonNode
 */
define('CDM_WS_PORTAL_TAXONOMY_CHILDNODES', 'portal/classification/$0/childNodes');
/**
 * returns list of TaxonNodeDto
 */
define('CDM_WS_PORTAL_TAXONOMY_CHILDNODES_AT_RANK', 'portal/classification/$0/childNodesAt/$1');
/**
 * returns list of TaxonNodeDto
 */
define('CDM_WS_PORTAL_TAXONOMY_CHILDNODES_OF_TAXON', 'portal/classification/$0/childNodesOf/$1');
/**
 * returns list of TaxonNodeDto
 */
define('CDM_WS_PORTAL_TAXONOMY_PATH_FROM', 'portal/classification/$0/pathFrom/$1');
/**
 * returns list of TaxonNodeDto
 */
define('CDM_WS_PORTAL_TAXONOMY_PATH_FROM_TO_RANK', 'portal/classification/$0/pathFrom/$1/toRank/$2');
define('CDM_WS_PORTAL_TAXONOMY_MEDIA', 'portal/classification/$0/$1');

define('CDM_WS_TAXONNODE_PARENT', 'taxonNode/$0/parent');

define('CDM_WS_TERMVOCABULARY', 'termVocabulary/$0');

define('CDM_WS_TERM_COMPARE', 'term/$0/compareTo/$1');
define('CDM_WS_TERM', 'term');
define('CDM_WS_PORTAL_TERM', 'portal/term/$0');


define('CDM_WS_TERMTREE', 'portal/termTree/$0');
define('CDM_WS_TERMTREES', 'portal/termTree');
// define('CDM_WS_TERMTREE_CHILDREN', 'termTree/$0/children');
define('CDM_WS_GEOSERVICE_DISTRIBUTIONMAP', 'ext/edit/mapServiceParameters/taxonDistributionFor/$0');
define('CDM_WS_GEOSERVICE_OCCURRENCEMAP_FOR_FIELDUNITS', 'ext/edit/mapServiceParameters/taxonOccurrencesForX');
define('CDM_WS_GEOSERVICE_OCCURRENCEMAP', 'ext/edit/mapServiceParameters/taxonOccurrencesFor/$0');
define('CDM_WS_KML_SPECIMENSOROCCURENCES', 'kml/specimensOrOccurences/$0');
define('CDM_WS_KML_TYPEDESIGNATIONS', 'kml/typeDesignations/$0');
define('CDM_WS_KML_TAXON_OCCURRENCE', 'kml/taxonOccurrencesFor/$0');

define('CDM_WS_OCCURRENCE', 'occurrence');
define('CDM_WS_OCCURRENCE_FIELD_UNIT_DTOS', 'occurrence/$0/fieldUnitDTOs');
define('CDM_WS_PORTAL_OCCURRENCE', 'portal/occurrence');
define('CDM_WS_PORTAL_OCCURRENCE_AS_DTO', 'portal/occurrence/$0/asDTO');
define('CDM_WS_OCCURRENCE_ACCESSION_NUMBER', 'occurrence/byGeneticAccessionNumber');
/**
 * url query parameters:
 *
 * - taxonUuid
 * - relationshipUuids e.g. CongruentTo;  "60974c98-64ab-4574-bb5c-c110f6db634d"
 * - relationshipInversUuids
 * - maxDepth null for unlimited
 * - pageNumber
 * - pageSize
 * - unknown_type
 *
 * returns Pager<SpecimenOrObservationBase>
 */
define('CDM_WS_OCCURRENCE_BY_ASSOCIATEDTAXON', 'occurrence/byAssociatedTaxon');
define('CDM_WS_OCCURRENCE_ROOTUNIT_DTO_BY_ASSOCIATEDTAXON', 'occurrence/rootUnitDTOsByAssociatedTaxon');
define('CDM_WS_DERIVEDUNIT_FACADE', 'derivedUnitFacade/$0');
define('CDM_WS_PORTAL_DERIVEDUNIT_FACADE', 'portal/derivedUnitFacade/$0');

define('CDM_WS_IDENTIFICATIONKEY', 'identificationKey');
define('CDM_WS_MEDIAKEY', 'mediaKey');
define('CDM_WS_MULTIACCESSKEY', 'multiAccessKey');
define('CDM_WS_POLYTOMOUSKEY', 'polytomousKey');
define('CDM_WS_PORTAL_POLYTOMOUSKEY_NODE', 'portal/polytomousKeyNode');

define('CDM_WS_MEDIA', 'media');
define('CDM_WS_PORTAL_MEDIA', 'portal/media');

define('CDM_WS_MEDIA_METADATA', 'media/$0/metadata');

/**
 * url query parameters:
 *  - identifier (mandatory)
 *
 */
define('CDM_WS_REGISTRATION_DTO', "registrationDTO");
/**
 * url query parameters:
 *  - identifier (mandatory)
 *
 */
define('CDM_WS_REGISTRATION_STATUS', "registration/status");

define('CDM_WS_MANAGE_REINDEX', 'manage/reindex');
define('CDM_WS_MANAGE_PURGE', 'manage/purge');

define('CDM_WS_EVENTBASE', 'eventBase');
