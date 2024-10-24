<?php
/**
 * @file
 * UUID definitions for usage with CDM Server.
 */

  define('UUID_DEFAULT_FEATURETREE', 'ac8d4e58-926d-4f81-ac77-cebdd295df7c');

  // TermVocabularies.
  define('UUID_LANGUAGE', '45ac7043-7f5e-4f37-92f2-3874aaaef2de');
  define('UUID_CONTINENT', 'e72cbcb6-58f8-4201-9774-15d0c6abc128');
  define('UUID_WATERBODY_OR_COUNTRY', '006b1870-7347-4624-990f-e5ed78484a1a');
  define('UUID_RANK', 'ef0d1ce1-26e3-4e83-b47b-ca74eed40b1b');
  define('UUID_SPECIMEN_TYPE_DESIGNATION_STATUS', 'ab177bd7-d3c8-4e58-a388-226fff6ba3c2');
  define('UUID_NOMENCLATURAL_STATUS_TYPE', 'bb28cdca-2f8a-4f11-9c21-517e9ae87f1f');
  define('UUID_SYNONYM_RELATIONSHIP_TYPE', '48917fde-d083-4659-b07d-413db843bd50');
  define('UUID_HYBRID_RELATIONSHIP_TYPE', 'fc4abe52-9c25-4cfa-a682-8615bf4bbf07');
  define('UUID_NAME_RELATIONSHIP_TYPE', '6878cb82-c1a4-4613-b012-7e73b413c8cd');
  define('UUID_TAXON_RELATIONSHIP_TYPE', '15db0cf7-7afc-4a86-a7d4-221c73b0c9ac');
  define('UUID_MARKER_TYPE', '19dffff7-e142-429c-a420-5d28e4ebe305');
  define('UUID_ANNOTATION_TYPE', 'ca04609b-1ba0-4d31-9c2e-aa8eb2f4e62d');
  define('UUID_NAMED_AREA_TYPE', 'e51d52d6-965b-4f7d-900f-4ba9c6f5dd33');
  define('UUID_NAMED_AREA_LEVEL', '49034253-27c8-4219-97e8-f8d987d3d122');
  define('UUID_FEATURE', 'b187d555-f06f-4d65-9e53-da7c93f8eaa8');
  define('UUID_TDWG_AREA', '1fb40504-d1d7-44b0-9731-374fbe6cac77');
  define('UUID_PRESENCE_ABSENCE_TERM', 'adbbbe15-c4d3-47b7-80a8-c7d104e53a05');
  define('UUID_SEX', '9718b7dd-8bc0-4cad-be57-3c54d4d432fe');
  define('UUID_DERIVATION_EVENT_TYPE', '398b50bb-348e-4fe0-a7f5-a75afd846d1f');
  define('UUID_PRESERVATION_METHOD', 'a7dc20c9-e6b3-459e-8f05-8d6d8fceb465');
  define('UUID_DETERMINATION_MODIFIER', 'fe87ea8d-6e0a-4e5d-b0da-0ab8ea67ca77');
  define('UUID_STATISTICAL_MEASURE', '066cc62e-7213-495e-a020-97a1233bc037');
  define('UUID_RIGHTS_TERM', '8627c526-73af-44d9-902c-11c1f11b60b4');
  define('UUID_NAME_TYPE_DESIGNATION_STATUS', 'ab60e738-4d09-4c24-a1b3-9466b01f9f55');
  define('UUID_EXTENSION_TYPE', '117cc307-5bd4-4b10-9b2f-2e14051b3b20');
  define('UUID_NAMEDAREALEVEL_TDWGLEVEL_1', 'cd7771b2-7427-4a01-9057-7d7a897dddaf');
  define('UUID_NAMEDAREALEVEL_TDWGLEVEL_2', '38efa5fd-d7f0-451c-9de9-e6cce41e2225');
  define('UUID_NAMEDAREALEVEL_TDWGLEVEL_3', '25b563b6-6a6c-401b-b090-c9498886c50b');
  define('UUID_NAMEDAREALEVEL_TDWGLEVEL_4', '160ff2c8-9bfc-49c2-9afd-049c21a91695');

  // Taxon RelationshipTypes.
  // (default)
  define('UUID_MISAPPLIED_NAME_FOR', '1ed87175-59dd-437e-959e-0d71583d8417');
  define('UUID_PROPARTE_MISAPPLIEDNAME_FOR', 'b59b4bd2-11ff-45d1-bae2-146efdeee206');
  define('UUID_PARTIAL_MISAPPLIEDNAME_FOR', '859fb615-b0e8-440b-866e-8a19f493cd36');
  define('UUID_PROPARTE_SYNONYM_FOR', '8a896603-0fa3-44c6-9cd7-df2d8792e577');
  define('UUID_PARTIAL_SYNONYM_FOR', '9d7a5e56-973c-474c-b6c3-a1cb00833a3c');
  // (optional)
  define('UUID_TAXONOMICALLY_INCLUDED_IN', 'd13fecdf-eb44-4dd7-9244-26679c05df1c');
  define('UUID_CONTRADICTION', 'a8f03491-2ad6-4fae-a04c-2a4c117a2e9b');
  define('UUID_CONGRUENT_TO', '60974c98-64ab-4574-bb5c-c110f6db634d');
  define('UUID_INCLUDES', '0501c385-cab1-4fbe-b945-fc747419bb13');
  define('UUID_OVERLAPS', '2046a0fd-4fd6-45a1-b707-2b91547f3ec7');
  define('UUID_EXCLUDES', '4535a63c-4a3f-4d69-9350-7bf02e2c23be');
  define('UUID_DOES_NOT_EXCLUDE', '0e5099bb-87c0-400e-abdc-bcfed5b5eece');
  define('UUID_DOES_NOT_OVERLAP', 'ecd2382b-3d94-4169-9dd2-2c4ea1d24605');
  define('UUID_NOT_INCLUDED_IN', '89dffa4e-e004-4d42-b0d1-ae1827529e43');
  define('UUID_NOT_CONGRUENT_TO', '6c16c33b-cfc5-4a00-92bd-a9f9e448f389');

  // SynonymTypes.
  define('UUID_SYNONYM_OF', '1afa5429-095a-48da-8877-836fa4fe709e');
  define('UUID_PRO_PARTE_SYNONYM_OF', '130b752d-2eff-4a62-a132-104ed8d13e5e');
  define('UUID_PARTIAL_SYNONYM_OF', '8b0d1d34-cc00-47cb-999d-b67f98d1af6e');
  define('UUID_HOMOTYPIC_SYNONYM_OF', '294313a9-5617-4ed5-ae2d-c57599907cb2');
  define('UUID_HETEROTYPIC_SYNONYM_OF', '4c1e2c59-ca55-41ac-9a82-676894976084');

  // NameRelationshipTypes.
  define('UUID_NAMERELATIONSHIPTYPE_ORTHOGRAPHIC_VARIANT', 'eeaea868-c4c1-497f-b9fe-52c9fc4aca53');
  define('UUID_NAMERELATIONSHIPTYPE_LATER_HOMONYM', '80f06f65-58e0-4209-b811-cb40ad7220a6');
  define('UUID_NAMERELATIONSHIPTYPE_TREATED_AS_LATER_HOMONYM', '2990a884-3302-4c8b-90b2-dfd31aaa2778');
  define('UUID_NAMERELATIONSHIPTYPE_ALTERNATIVE_NAME', '049c6358-1094-4765-9fae-c9972a0e7780');
  define('UUID_NAMERELATIONSHIPTYPE_BASIONYM', '25792738-98de-4762-bac1-8c156faded4a');
  define('UUID_NAMERELATIONSHIPTYPE_REPLACED_SYNONYM', '71c67c38-d162-445b-b0c2-7aba56106696');
  define('UUID_NAMERELATIONSHIPTYPE_CONSERVED_AGAINST', 'e6439f95-bcac-4ebb-a8b5-69fa5ce79e6a');
  define('UUID_NAMERELATIONSHIPTYPE_VALIDATED_BY_NAME', 'a176c9ad-b4c2-4c57-addd-90373f8270eb');
  define('UUID_NAMERELATIONSHIPTYPE_LATER_VALIDATED_BY_NAME', 'a25ee4c1-863a-4dab-9499-290bf9b89639');
  define('UUID_NAMERELATIONSHIPTYPE_BLOCKING_NAME_FOR', '1dab357f-2e12-4511-97a4-e5153589e6a6');
  define('UUID_NAMERELATIONSHIPTYPE_MISSPELLING', 'c6f9afcb-8287-4a2b-a6f6-4da3a073d5de');
  define('UUID_NAMERELATIONSHIPTYPE_AVOIDS_HOMONYM_OF', 'c7d59ab9-a8c6-4645-a990-04c698f2c123');
  define('UUID_NAMERELATIONSHIPTYPE_IS_NOT', '78360e2a-159d-4e2f-893e-8666805840fa');

  // Features.
  define('UUID_UNKNOWN', '910307f1-dc3c-452c-a6dd-af5ac7cd365c');
  define('UUID_DESCRIPTION', '9087cdcd-8b08-4082-a1de-34c9ba9fb493');
  define('UUID_DISTRIBUTION', '9fc9d10c-ba50-49ee-b174-ce83fc3f80c6');
  define('UUID_ECOLOGY', 'aa923827-d333-4cf5-9a5f-438ae0a4746b');
  define('UUID_BIOLOGY_ECOLOGY', '9832e24f-b670-43b4-ac7c-20a7261a1d8c');
  define('UUID_KEY', 'a677f827-22b9-4205-bb37-11cb48dd9106');
  define('UUID_MATERIALS_EXAMINED', '7c0c7571-a864-47c1-891d-01f59000dae1');
  define('UUID_MATERIALS_METHODS', '1e87d9c3-0844-4a03-9686-773e2ccb3ab6');
  define('UUID_ETYMOLOGY', 'dd653d48-355c-4aec-a4e7-724f6eb29f8d');
  define('UUID_DIAGNOSIS', 'd43d8501-ceab-4caa-9e51-e87138528fac');
  define('UUID_PROTOLOG', '7f1fd111-fc52-49f0-9e75-d0097f576b2d');
  define('UUID_COMMON_NAME', 'fc810911-51f0-4a46-ab97-6562fe263ae5');
  define('UUID_PHENOLOGY', 'a7786d3e-7c58-4141-8416-346d4c80c4a2');
  define('UUID_OCCURRENCE', '5deff505-1a32-4817-9a74-50e6936fd630');
  define('UUID_CITATION', '99b2842f-9aa7-42fa-bd5f-7285311e0101');
  define('UUID_USES', 'e5374d39-b210-47c7-bec1-bee05b5f1cb6');
  define('UUID_USE_SUMMARY', 'e6bd0bb0-7b1a-11e0-819a-0800200c9a66');
  define('UUID_USE', '6acb0348-c070-4512-a37c-67bcac016279');
  define('UUID_USE_RECORD', '8125a59d-b4d5-4485-89ea-67306297b599');
  const UUID_ADDITIONAL_PUBLICATION = '2c355c16-cb04-4858-92bf-8da8d56dea95';
  define('UUID_CONSERVATION', '4518fc20-2492-47de-b345-777d2b83c9cf');
  define('UUID_CULTIVATION', 'e28965b2-a367-48c5-b954-8afc8ac2c69b');
  define('UUID_INTRODUCTION', 'e75255ca-8ff4-4905-baad-f842927fe1d3');
  define('UUID_DISCUSSION', 'd3c4cbb6-0025-4322-886b-cd0156753a25');
  define('UUID_IMAGE', '84193b2c-327f-4cce-90ef-c8da18fd5bb5');
  /*
   * UUID_CHROMOSOMES used in flora_malesiana & flora afrique
   * is semantically the same as UUID_CHROMOSOMES_NUMBERS
   */
  define('UUID_CHROMOSOMES', 'c4a60319-4978-4692-9545-58d60cf8379e');
  define('UUID_CHROMOSOMES_NUMBERS', '6f677e98-d8d5-4bc5-80bf-affdb7e3945a');


  // SpecimenTypeDesignationStatus.
  define('UUID_STD_HOLOTYPE', 'a407dbc7-e60c-46ff-be11-eddf4c5a970d');
  define('UUID_STD_LECTOTYPE', '05002d46-083e-4b27-8731-2e7c28a8825c');
  define('UUID_STD_NEOTYPE', '26e13359-8f77-4e40-a85a-56c01782fce0');
  define('UUID_STD_EPITYPE', '989a2715-71d5-4fbe-aa9a-db9168353744');
  define('UUID_STD_ISOLECTOTYPE', '7a1a8a53-78f4-4fc0-89f7-782e94992d08');
  define('UUID_STD_ISONEOTYPE', '7afc2f4f-f70a-4aa5-80a5-87764f746bde');
  define('UUID_STD_ISOTYPE', '93ef8257-0a08-47bb-9b36-542417ae7560');
  define('UUID_STD_PARANEOTYPE', '0c39e2a5-2fe0-4d4f-819a-f609b5340339');
  define('UUID_STD_PARATYPE', 'eb7df2e5-d9a7-479d-970c-c6f2b0a761d7');
  define('UUID_STD_SECONDSTEPLECTOTYPE', '01d91053-7004-4984-aa0d-9f4de59d6205');
  define('UUID_STD_SECONDSTEPNEOTYPE', '8d2fed1f-242e-4bcf-bbd7-e85133e479dc');
  define('UUID_STD_SYNTYPE', 'f3b60bdb-4638-4ca9-a0c7-36e77d8459bb');
  define('UUID_STD_PARALECTOTYPE', '7244bc51-14d8-41a6-9524-7dc5303bba29');
  define('UUID_STD_ISOEPITYPE', '95b90696-e103-4bc0-b60b-c594983fb566');
  define('UUID_STD_ICONOTYPE', '643513d0-32f5-46ba-840b-d9b9caf8160f');
  define('UUID_STD_PHOTOTYPE', 'b7807acc-f559-474e-ad4a-e7a41e085e34');
  define('UUID_STD_UNSPECIFIC', '230fd762-b143-49de-ac2e-744bcc48a63b');
  define('UUID_STD_ORIGINALMATERIAL', '49c96cae-6be6-401e-9b36-1bc12d9dc8f9');

  // NameTypeDesignationStatus.
  define('UUID_NTD_AUTOMATIC', "e89d8b21-615a-4602-913f-1625bf39a69f");
  define('UUID_NTD_MONOTYPY', "3fc639b2-9a64-45f8-9a81-657a4043ad74");
  define('UUID_NTD_NOT APPLICABLE', "91a9d6a9-7754-41cd-9f7e-be136f599f7e");
  define('UUID_NTD_ORIGINAL_DESIGNATION', "40032a44-973b-4a64-b25e-76f86c3a753c");
  define('UUID_NTD_PRESENT_DESIGNATION', "e5f38f5d-995d-4470-a036-1a9792a543fc");
  define('UUID_NTD_SUBSEQUENT_MONOTYPY', "2b5806d8-31b0-406e-a32a-4adac0c89ae4");
  define('UUID_NTD_SUBSEQUENT_DESIGNATION', "3e449e7d-a03c-4431-a7d3-aa258406f6b2");
  define('UUID_NTD_TAUTONYMY', "84521f09-3e10-43f5-aa6f-2173a55a6790");
  define('UUID_NTD_LECTOTYPE', "4177c938-b741-40e1-95e5-4c53bd1ed87d");

  // Rights terms.
  define('UUID_RIGHTS_LICENCE', '67c0d47e-8985-1014-8845-c84599f9992c');
  define('UUID_RIGHTS_COPYRIGHT', 'd1ef838e-b195-4f28-b8eb-0d3be080bd37');
  define('UUID_RIGHTS_ACCESS_RIGHTS', 'a50b4def-b3ac-4508-b50a-e0f249e3a1d7');

  // MarkerType terms.
  define('UUID_MARKERTYPE_USE', '2e6e42d9-e92a-41f4-899b-03c0ac64f039');
  define('UUID_MARKERTYPE_COMPUTED', '5cc15a73-2947-44e3-9319-85dd20736e55');

  // Nomenclatural status types.
  define('UUID_NOMENCLATURALSTATUS_TYPE_UUIDAMBIGUOUS', '90f5012b-705b-4488-b4c6-002d2bc5198e');
  define('UUID_NOMENCLATURALSTATUS_TYPE_DOUBTFUL', '0ffeb39e-872e-4c0f-85ba-a4150d9f9e7d');
  define('UUID_NOMENCLATURALSTATUS_TYPE_CONFUSUM', '24955174-aa5c-4e71-a2fd-3efc79e885db');
  define('UUID_NOMENCLATURALSTATUS_TYPE_ILLEGITIMATE', 'b7c544cf-a375-4145-9d3e-4b97f3f18108');
  define('UUID_NOMENCLATURALSTATUS_TYPE_SUPERFLUOUS', '6890483a-c6ba-4ae1-9ab1-9fbaa5736ce9');
  define('UUID_NOMENCLATURALSTATUS_TYPE_REJECTED', '48107cc8-7a5b-482e-b438-efbba050b851');
  define('UUID_NOMENCLATURALSTATUS_TYPE_UTIQUEREJECTED', '04338fdd-c12a-402f-a1ca-68b4bf0be042');
  define('UUID_NOMENCLATURALSTATUS_TYPE_CONSERVEDPROP', '82bab006-5aed-4301-93ec-980deb30cbb1');
  define('UUID_NOMENCLATURALSTATUS_TYPE_ORTHOGRAPHYCONSERVEDPROP', '02f82bc5-1066-454b-a023-11967cba9092');
  define('UUID_NOMENCLATURALSTATUS_TYPE_LEGITIMATE', '51a3613c-b53b-4561-b0cd-9163d91c15aa');
  define('UUID_NOMENCLATURALSTATUS_TYPE_ALTERNATIVE', '3b8a8519-420f-4dfa-b050-b410cc257961');
  define('UUID_NOMENCLATURALSTATUS_TYPE_NOVUM', '05fcb68f-af60-4851-b912-892512058897');
  define('UUID_NOMENCLATURALSTATUS_TYPE_UTIQUEREJECTEDPROP', '643ee07f-026c-426c-b838-c778c8613383');
  define('UUID_NOMENCLATURALSTATUS_TYPE_ORTHOGRAPHYCONSERVED', '34a7d383-988b-4117-b8c0-52b947f8c711');
  define('UUID_NOMENCLATURALSTATUS_TYPE_REJECTEDPROP', '248e44c2-5436-4526-a352-f7467ecebd56');
  define('UUID_NOMENCLATURALSTATUS_TYPE_CONSERVED', '6330f719-e2bc-485f-892b-9f882058a966');
  define('UUID_NOMENCLATURALSTATUS_TYPE_SANCTIONED', '1afe55c4-76aa-46c0-afce-4dc07f512733');
  define('UUID_NOMENCLATURALSTATUS_TYPE_INVALID', 'b09d4f51-8a77-442a-bbce-e7832aaf46b7');
  define('UUID_NOMENCLATURALSTATUS_TYPE_NUDUM', 'e0d733a8-7777-4b27-99a3-05ab50e9f312');
  define('UUID_NOMENCLATURALSTATUS_TYPE_COMBINATIONINVALID', 'f858e619-7b7f-4225-913b-880a2143ec83');
  define('UUID_NOMENCLATURALSTATUS_TYPE_PROVISIONAL', 'a277507e-ad93-4978-9419-077eb889c951');
  define('UUID_NOMENCLATURALSTATUS_TYPE_VALID', 'bd036217-5499-4ccd-8f4c-72e06158db93');
  define('UUID_NOMENCLATURALSTATUS_TYPE_SUBNUDUM', '92a76bd0-6ea8-493f-98e0-4be0b98c092f');

  // Annotation type vocabulary.
  define('UUID_ANNOTATION_TYPE_TECHNICAL', '6a5f9ea4-1bdd-4906-89ad-6e669f982d69');
  define('UUID_ANNOTATION_TYPE_EDITORIAL','e780d5fd-abfc-4025-938a-46deb751d808');
  define('UUID_ANNOTATION_TYPE_UNTYPED','3ccf04c8-2739-43ad-ab53-de4b83b56e8b');

  // DerivationEventType
  define('UUID_DERIVATIONEVENTTYPE_DUPLICATE', '8f54c7cc-eb5e-4652-a6e4-3a4ba429b327');
  define('UUID_DERIVATIONEVENTTYPE_GATHERING_IN_SITU', '1cb2bd40-5c9c-459b-89c7-4d9c2fca7432');
  define('UUID_DERIVATIONEVENTTYPE_TISSUE_SAMPLING', '9dc1df08-1f31-4008-a4e2-1ddf7c9115da');
  define('UUID_DERIVATIONEVENTTYPE_DNA_EXTRACTION', 'f9f957b6-88c0-4531-9a7f-b5fb1c9daf66');
  define('UUID_DERIVATIONEVENTTYPE_VEGETATIV_PROPAGATION', 'a4a8e4ce-0e58-462a-be67-a7f567d96da1');
  define('UUID_DERIVATIONEVENTTYPE_DUPLICATE_SEGREGATION', '661e7292-6bcb-495d-a3cc-140024ae3471');
  define('UUID_DERIVATIONEVENTTYPE_ACCESSIONING', '3c7c0929-0528-493e-9e5f-15e0d9585fa1');
  define('UUID_DERIVATIONEVENTTYPE_SEXUAL_REPRODUCTION', 'aa79baac-165d-47ad-9e80-52a03776d8ae');

  // StatisticalMeasure
define('UUID_STATISTICALMEASURE_MIN', "2c8b42e5-154c-42bd-a301-03b483275dd6");
define('UUID_STATISTICALMEASURE_MAX', "8955815b-7d21-4149-b1b7-d37af3c2046c");
define('UUID_STATISTICALMEASURE_AVERAGE', "264c3979-d551-4795-9e25-24c6b533fbb1");
define('UUID_STATISTICALMEASURE_SAMPLESIZE', "571f86ca-a44c-4484-9981-11fd82138a7a");
define('UUID_STATISTICALMEASURE_VARIANCE', "4d22cf5e-89ff-4de3-a9ae-12dbeda3faba");
define('UUID_STATISTICALMEASURE_TYPICALLOWERBOUNDARY', "8372a89a-35ad-4755-a881-7edae6c37c8f");
define('UUID_STATISTICALMEASURE_TYPICALUPPERBOUNDARY', "9eff88ba-b8e7-4631-9e55-a50bd16ba79d");
define('UUID_STATISTICALMEASURE_STANDARDDEVIATION', "9ee4397e-3496-4fe1-9114-afc7d7bdc65");
define('UUID_STATISTICALMEASURE_EXACTVALUE', "29736701-58c4-48b3-a9d7-41c74140cac7");
define('UUID_STATISTICALMEASURE_STATISTICALMEASUREUNKNOWNDATA', "4bbd6e78-6d4e-4ec8-ac14-12f53aae049e");

// IdentifierType
const UUID_IDENTIFIER_TYPE_LSID = '26729412-9df6-4cc3-9e5d-501531ca21f0';

// SpecimenOrObservationType
const UUID_SPECIMEN_OR_OBSERVATION_TYPE_UNKNOWN = "971a0c72-d4d2-4e41-8520-c9a87df34f48";
const UUID_SPECIMEN_OR_OBSERVATION_TYPE_DERIVEDUNIT = "da80443a-360b-4861-abeb-21e13beb5186";
const UUID_SPECIMEN_OR_OBSERVATION_TYPE_PRESERVEDSPECIMEN = "95cd9246-4131-444f-ad2f-3b24ca294a1f";
const UUID_SPECIMEN_OR_OBSERVATION_TYPE_FOSSIL = "1b0f8534-35eb-4c64-8e53-69e734043bd6";
const UUID_SPECIMEN_OR_OBSERVATION_TYPE_LIVING_SPECIMEN = "bc46169e-4d31-4eae-b5aa-1ddf0520c9a9";
const UUID_SPECIMEN_OR_OBSERVATION_TYPE_OTHER_SPECIMEN = "b636da6a-b48f-4084-9594-25ea82429b70";
const UUID_SPECIMEN_OR_OBSERVATION_TYPE_OBSERVATION = "a8a254f1-7bed-47ec-bbee-86a794819c3b";
const UUID_SPECIMEN_OR_OBSERVATION_TYPE_HUMAN_OBSERVATION = "b960c06d-4bfc-4bea-bc53-aec0600409b1";
const UUID_SPECIMEN_OR_OBSERVATION_TYPE_MACHINE_OBSERVATION = "b12a13fc-0f61-4055-b9b7-4eabd417c54c";
const UUID_SPECIMEN_OR_OBSERVATION_TYPE_TISSUE_SAMPLE = "3ad39d74-9bb3-4f9c-b261-8f5637bef582";
const UUID_SPECIMEN_OR_OBSERVATION_TYPE_DNA_SAMPLE = "6a724560-bdfa-41c9-b459-ab0f1fc74902";
const UUID_SPECIMEN_OR_OBSERVATION_TYPE_MEDIA = "0efa6b3e-e67a-49d4-a758-f3fc688901a7";
const UUID_SPECIMEN_OR_OBSERVATION_TYPE_STILL_IMAGE = "a8d9ada5-7f22-4fcf-8693-ae68d527289b";
const UUID_SPECIMEN_OR_OBSERVATION_TYPE_MOVING_IMAGE = "56722418-9398-4367-afa1-46982fb93959";
const UUID_SPECIMEN_OR_OBSERVATION_TYPE_SOUND_RECORDING = "2a39ec19-4aae-4b74-bc5c-578c5dc94e7d";
const UUID_SPECIMEN_OR_OBSERVATION_TYPE_MULTIMEDIA = "bfe3fef8-d294-4554-847a-c9d8a6b74313";
const UUID_SPECIMEN_OR_OBSERVATION_TYPE_MATERIAL_SAMPLE = "d6395063-63b3-485f-87d1-8b2eaf224a33";
const UUID_SPECIMEN_OR_OBSERVATION_TYPE_FIELD_UNIT = "d38d22db-17f9-45ba-a32f-32393788726f";





