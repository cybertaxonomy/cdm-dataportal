<?php
/**
 * @file
 * CDM Server URI definitions.
 */

const CDM_WS_CLASSIFICATION = 'classification';

const CDM_WS_AGENT = 'agent';
const CDM_WS_PORTAL_AGENT = 'portal/agent';
const CDM_WS_REFERENCE = 'reference';
const CDM_WS_REFERENCE_AUTHORTEAM = 'reference/$0/authorship';
/**
 * Parameter $0 : the reference uuid
 *
 * Provides the protologue and original publication links of the name
 */
const CDM_WS_NAME_PROTOLOGUE_LINKS = 'name/$0/protologueLinks';
const CDM_WS_NOMENCLATURAL_REFERENCE_CITATION = 'reference/$0/nomenclaturalCitation';

const CDM_WS_NAME = 'name';
const CDM_WS_NAME_NAMECAHE = 'name/$0/nameCache';
const CDM_WS_NAME_TYPEDESIGNATIONS = 'name/$0/typeDesignations';
const CDM_WS_TYPE_DESIGNATION_STATUS_FILTER_TERMS = 'name/typeDesignationStatusFilterTerms';

const CDM_WS_PORTAL_NAME = 'portal/name';
const CDM_WS_PORTAL_NAME_FINDBYNAME = 'name/findByName/';
const CDM_WS_PORTAL_NAME_DESCRIPTIONS = 'portal/name/$0/taxonNameDescriptions';
const CDM_WS_PORTAL_NAME_TYPEDESIGNATIONS = 'portal/name/$0/typeDesignations';
const CDM_WS_PORTAL_NAME_TYPEDESIGNATIONS_IN_HOMOTYPICAL_GROUP = 'portal/name/$0/typeDesignationsInHomotypicalGroup';
const CDM_WS_PORTAL_NAME_NAME_RELATIONS = 'portal/name/$0/nameRelations';

const CDM_WS_NOMENCLATURALSTATUS = 'nomenclaturalStatus/$0';

const CDM_WS_TYPEDESIGNATION = 'typedesignation/$0';
const CDM_WS_PORTAL_TYPEDESIGNATION = 'portal/typedesignation/$0';


const CDM_WS_TAXON = 'taxon';

const CDM_WS_PORTAL_TAXON_ROOTUNIT_DTOS = 'portal/taxon/$0/rootUnitDTOs';

const CDM_WS_TAXON_CLASSIFICATIONS = 'taxon/$0/classifications';
const CDM_WS_TAXON_FINDBY_DESCRIPTIONELEMENT_FULLTEXT = 'taxon/findByDescriptionElementFullText';
/**
 * Parameter $0 : the taxon uuid.
 *
 * Returns the taxon which is the accepted synonym for the taxon given as
 * parameter taxonUuid. If the taxon specified by taxonUuid is itself the
 * accepted taxon, this one will be returned.
 */
const CDM_WS_PORTAL_TAXON_ACCEPTED = 'portal/taxon/$0/accepted';

const CDM_WS_PORTAL_TAXON = 'portal/taxon';
const CDM_WS_PORTAL_TAXON_DTO = 'portal/taxon/$0/page';
const CDM_WS_PORTAL_TAXON_SYNONYMY = 'portal/taxon/$0/synonymy';
const CDM_WS_PORTAL_TAXON_RELATIONS = 'portal/taxon/$0/taxonRelationships';
const CDM_WS_PORTAL_TAXON_RELATIONS_DTO = 'taxon/$0/taxonRelationshipsDTO';
const CDM_WS_PORTAL_TAXON_DESCRIPTIONS = 'portal/taxon/$0/descriptions';
const CDM_WS_PORTAL_TAXON_TAXONNODEAGENTRELATIONS = 'portal/taxon/$0/taxonNodeAgentRelations/$1';

/**
 * Parameter $0: taxon UUID.
 * Parameter $1: simple name of the class extending DescriptionElementBase.
 */
const CDM_WS_PORTAL_TAXON_DESCRIPTIONS_ELEMENTSBYTYPE = 'portal/taxon/$0/descriptions/elementsByType/$1';
const CDM_WS_PORTAL_TAXON_USEDESCRIPTIONS = 'portal/taxon/$0/useDescriptions';
const CDM_WS_PORTAL_TAXON_ASSOCIATED_ROOTUNITS = 'portal/taxon/$0/associatedRootUnits';
const CDM_WS_PORTAL_TAXON_TO_NAMERELATIONS = 'portal/taxon/$0/toNameRelationships';
const CDM_WS_PORTAL_TAXON_FROM_NAMERELATIONS = 'portal/taxon/$0/fromNameRelationships';
const CDM_WS_PORTAL_TAXON_MEDIA = 'portal/taxon/$0/media';
const CDM_WS_PORTAL_TAXON_SUBTREE_MEDIA = 'portal/taxon/$0/subtree/media';

const CDM_WS_PORTAL_TAXON_TAXONNODES = 'portal/taxon/$0/taxonNodes';
const CDM_WS_PORTAL_TAXON_FIND = 'portal/taxon/findDto';
const CDM_WS_PORTAL_TAXON_SEARCH = 'portal/taxon/search';
const CDM_WS_TAXON_SEARCH = 'taxon/search';
const CDM_WS_PORTAL_TAXON_FINDBY_DESCRIPTIONELEMENT_FULLTEXT = 'portal/taxon/findByDescriptionElementFullText';

const CDM_WS_IIIF_TAXON_MANIFEST = 'iiif/taxon/$0/manifest';


const CDM_WS_TAXONNODE = 'taxonNode/$0';

/**
 * /description/{uuid}/naturalLanguageDescription/{termtree_uuid}
 */
const CDM_WS_DESCRIPTION_NAMEDAREAS_IN_USE = 'description/namedAreasInUse';
const CDM_WS_DESCRIPTION_NATURALLANGUAGE_DESCRIPTION = 'description/$0/naturalLanguageDescription/$1';
const CDM_WS_DESCRIPTION_HAS_STRUCTRURED_DATA = 'description/$0/hasStructuredData';

const CDM_WS_PORTAL_DESCRIPTION = 'portal/description/$0';
const CDM_WS_PORTAL_DESCRIPTION_AREAS_TREE = 'portal/description/$0/namedAreaTree';

const CDM_WS_PORTAL_DESCRIPTION_DISTRIBUTION_INFO_FOR = 'portal/description/distributionInfoFor/$0';

const CDM_WS_DESCRIPTIONELEMENT = 'descriptionElement/$0';
const CDM_WS_DESCRIPTIONELEMENT_BY_TAXON = 'portal/descriptionElement/byTaxon';
const CDM_WS_PORTAL_DESCRIPTIONELEMENT = 'portal/descriptionElement/$0';

/**
 * Gets the root nodes of the taxonomic concept tree for the concept
 * reference specified by the secUuid parameter.
 *
 * stub: treenode_root
 */
const CDM_WS_PORTAL_TAXONOMY = 'portal/classification';
/**
 * TODO: harmonize return type, the REST service should return TaxonNodeDto, see #6222
 *
 * returns list of TaxonNode
 */
const CDM_WS_PORTAL_TAXONOMY_CHILDNODES = 'portal/classification/$0/childNodes';
/**
 * returns list of TaxonNodeDto
 */
const CDM_WS_PORTAL_TAXONOMY_CHILDNODES_AT_RANK = 'portal/classification/$0/childNodesAt/$1';
/**
 * returns list of TaxonNodeDto
 */
const CDM_WS_PORTAL_TAXONOMY_CHILDNODES_OF_TAXON = 'portal/classification/$0/childNodesOf/$1';
/**
 * returns list of TaxonNodeDto
 */
const CDM_WS_PORTAL_TAXONOMY_PATH_FROM = 'portal/classification/$0/pathFrom/$1';
/**
 * returns list of TaxonNodeDto
 */
const CDM_WS_PORTAL_TAXONOMY_PATH_FROM_TO_RANK = 'portal/classification/$0/pathFrom/$1/toRank/$2';
const CDM_WS_PORTAL_TAXONOMY_MEDIA = 'portal/classification/$0/$1';

const CDM_WS_TAXONNODE_PARENT = 'taxonNode/$0/parent';

const CDM_WS_TERMVOCABULARY = 'termVocabulary/$0';

const CDM_WS_TERM_COMPARE = 'term/$0/compareTo/$1';
const CDM_WS_TERM = 'term';
const CDM_WS_PORTAL_TERM = 'portal/term/$0';


const CDM_WS_TERMTREE = 'portal/termTree/$0';
const CDM_WS_TERMTREES = 'portal/termTree';
// define('CDM_WS_TERMTREE_CHILDREN', 'termTree/$0/children');
const CDM_WS_GEOSERVICE_OCCURRENCEMAP_FOR_FIELDUNITS = 'ext/edit/mapServiceParameters/taxonOccurrencesForX';
const CDM_WS_GEOSERVICE_OCCURRENCEMAP = 'ext/edit/mapServiceParameters/taxonOccurrencesFor/$0';
const CDM_WS_KML_SPECIMENSOROCCURENCES = 'kml/specimensOrOccurences/$0';
const CDM_WS_KML_TYPEDESIGNATIONS = 'kml/typeDesignations/$0';
const CDM_WS_KML_TAXON_OCCURRENCE = 'kml/taxonOccurrencesFor/$0';

const CDM_WS_OCCURRENCE = 'occurrence';
const CDM_WS_OCCURRENCE_ROOT_UNIT_DTOS = 'occurrence/$0/rootUnitDTOs';
const CDM_WS_PORTAL_OCCURRENCE = 'portal/occurrence';
const CDM_WS_PORTAL_OCCURRENCE_AS_DTO = 'portal/occurrence/$0/asDTO';
const CDM_WS_OCCURRENCE_ACCESSION_NUMBER = 'occurrence/byGeneticAccessionNumber';
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
const CDM_WS_OCCURRENCE_BY_ASSOCIATEDTAXON = 'occurrence/byAssociatedTaxon';
const CDM_WS_OCCURRENCE_ROOTUNIT_DTO_BY_ASSOCIATEDTAXON = 'occurrence/rootUnitDTOsByAssociatedTaxon';
const CDM_WS_DERIVEDUNIT_FACADE = 'derivedUnitFacade/$0';
const CDM_WS_PORTAL_DERIVEDUNIT_FACADE = 'portal/derivedUnitFacade/$0';

const CDM_WS_IDENTIFICATIONKEY = 'identificationKey';
const CDM_WS_MEDIAKEY = 'mediaKey';
const CDM_WS_MULTIACCESSKEY = 'multiAccessKey';
const CDM_WS_POLYTOMOUSKEY = 'polytomousKey';
const CDM_WS_PORTAL_POLYTOMOUSKEY_NODE = 'portal/polytomousKeyNode';

const CDM_WS_MEDIA = 'media';
const CDM_WS_PORTAL_MEDIA = 'portal/media';

const CDM_WS_MEDIA_METADATA = 'media/$0/metadata';

const CDM_WS_REGISTRATION = "registration";

/**
 * url query parameters:
 *  - identifier (mandatory)
 *
 */
const CDM_WS_REGISTRATION_DTO = "registrationDTO";
/**
 * url query parameters:
 *  - identifier (mandatory)
 *
 */
const CDM_WS_REGISTRATION_STATUS = "registration/status";

const CDM_WS_MANAGE_REINDEX = 'manage/reindex';
const CDM_WS_MANAGE_PURGE = 'manage/purge';

const CDM_WS_EVENTBASE = 'eventBase';
