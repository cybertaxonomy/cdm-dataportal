<?php


class NameRenderConfiguration {

  const DEFAULT_CONFIGURATION = 0;
  const CUSTOM_CONFIGURATION = 1;
  const PRE380_CONFIGURATION = 2;

  const CDM_PART_DEFINITIONS = 'cdm-part-definitions';
  const CDM_NAME_RENDER_TEMPLATES = 'cdm-name-render-templates';

  const CDM_PART_DEFINITIONS_DEFAULT =
  array(
    'ZoologicalName' => array(
      'namePart' => array('name' => TRUE),
      'nameAuthorPart' => array('name' => TRUE),
      'referencePart' => array('authors' => TRUE),
      'microreferencePart' => array('microreference' => TRUE),
      'secReferencePart' => array('secReference' => TRUE,),
      'statusPart' => array('status' => TRUE),
      'descriptionPart' => array('description' => TRUE),
    ),
    'BotanicalName'=> array(
      'namePart' => array('name' => TRUE),
      'nameAuthorPart' => array('name' => TRUE, 'authors' => TRUE),
      'referencePart' => array('reference' => TRUE, 'microreference' => TRUE),
      'secReferencePart' => array('secReference' => TRUE,),
      'referenceYearPart' => array('reference.year' => TRUE),
      'statusPart' => array('status' => TRUE),
      'descriptionPart' => array('description' => TRUE),
    ),
    '#DEFAULT' => array(
      'namePart' => array(
        'name' => TRUE
      ),
      'nameAuthorPart' => array(
        'name' => TRUE,
        'authors' => TRUE
      ),
      'referencePart' => array(
        'reference' => TRUE
      ),
      'secReferencePart' => array(
        'secReference' => TRUE,
      ),
      'microreferencePart' => array(
        'microreference' => TRUE,
      ),
      'statusPart' => array(
        'status' => TRUE,
      ),
      'descriptionPart' => array(
        'description' => TRUE,
      ),
    )
  );

  const CDM_PART_DEFINITIONS_DEFAULT_PRE_380 =
    array(
      'ZoologicalName' => array(
        'namePart' => array('name' => TRUE),
        'nameAuthorPart' => array('name' => TRUE),
        'referencePart' => array('authors' => TRUE),
        'microreferencePart' => array('microreference' => TRUE),
        'statusPart' => array('status' => TRUE),
        'descriptionPart' => array('description' => TRUE),
      ),
      'BotanicalName'=> array(
        'namePart' => array('name' => TRUE),
        'nameAuthorPart' => array('name' => TRUE, 'authors' => TRUE),
        'referencePart' => array('reference' => TRUE, 'microreference' => TRUE),
        'referenceYearPart' => array('reference.year' => TRUE),
        'statusPart' => array('status' => TRUE),
        'descriptionPart' => array('description' => TRUE),
      ),
      '#DEFAULT' => array(
        'namePart' => array(
          'name' => TRUE
        ),
        'nameAuthorPart' => array(
          'name' => TRUE,
          'authors' => TRUE
        ),
        'referencePart' => array(
          'reference' => TRUE
        ),
        'microreferencePart' => array(
          'microreference' => TRUE,
        ),
        'statusPart' => array(
          'status' => TRUE,
        ),
        'descriptionPart' => array(
          'description' => TRUE,
        ),
      )
    );

  const CDM_NAME_RENDER_TEMPLATES_DEFAULT =
    array (
      'taxon_page_title,polytomousKey'=> array(
        'namePart' => array('#uri' => TRUE),
      ),
      'not_in_current_classification' => array(
        'nameAuthorPart' => TRUE,
        'referencePart' => TRUE,
        'statusPart' => TRUE,
        'secReferencePart' => TRUE,
      ),
      'taxon_page_synonymy,accepted_taxon.taxon_page_synonymy,name_page,registration_page'=> array(
        'nameAuthorPart' => array('#uri' => TRUE),
        'referencePart' => TRUE,
        'descriptionPart' => TRUE,
        'statusPart' => TRUE,
      ),
      'related_taxon.other_taxon_relationship.taxon_relationships.taxon_page_synonymy'=> array(
        'nameAuthorPart' => array('#uri' => TRUE),
        'referencePart' => TRUE,
        'descriptionPart' => TRUE,
        'statusPart' => TRUE,
        'secReferencePart' => TRUE,
      ),
      'related_taxon.misapplied_name_for.taxon_relationships.taxon_page_synonymy' => array(
        'nameAuthorPart' => array('#uri' => TRUE),
        'referencePart' => TRUE,
        'descriptionPart' => TRUE,
        'statusPart' => TRUE,
        /* no sec ref in this case, misapplied names are
         * de-duplicated and the sec ref is shown as footnote */
      ),
      'homonym'=> array(
        'nameAuthorPart' => array('#uri' => TRUE),
        'referenceYearPart' => TRUE,
        'statusPart' => TRUE
      ),
      'acceptedFor,typedesignations,list_of_taxa' => array(
        'nameAuthorPart' => array('#uri' => TRUE),
        'referencePart' => TRUE,
        'statusPart' => TRUE
      ),
      '#DEFAULT' => array(
        'nameAuthorPart' => array('#uri' => TRUE),
        'referencePart' => TRUE,
        'statusPart' => TRUE
      )
    );

  const CDM_NAME_RENDER_TEMPLATES_DEFAULT_PRE_380 =
    array (
      'taxon_page_title,polytomousKey'=> array(
        'namePart' => array('#uri' => TRUE),
      ),
      'taxon_page_synonymy,related_taxon'=> array(
        'nameAuthorPart' => array('#uri' => TRUE),
        'referencePart' => TRUE,
        'statusPart' => TRUE,
        'descriptionPart' => TRUE,
      ),
      'homonym'=> array(
        'nameAuthorPart' => array('#uri' => TRUE),
        'referenceYearPart' => TRUE,
      ),
      'acceptedFor,typedesignations,list_of_taxa' => array(
        'nameAuthorPart' => array('#uri' => TRUE),
        'referencePart' => TRUE,
      ),
      '#DEFAULT' => array(
        'nameAuthorPart' => array('#uri' => TRUE),
        'referencePart' => TRUE,
      )
    );

  private $default_part_definition_json = null;
  private $current_part_definition_json = null;


  private $default_render_templates_json = null;
  private $current_render_templates_json = null;

  function partDefinitionConfigurationStatus(){

    $default_part_definitions_pre_380_json = json_encode(self::CDM_PART_DEFINITIONS_DEFAULT_PRE_380, JSON_PRETTY_PRINT);
    $this->default_part_definition_json = json_encode(self::CDM_PART_DEFINITIONS_DEFAULT, JSON_PRETTY_PRINT);
    $this->current_part_definition_json = json_encode(variable_get(self::CDM_PART_DEFINITIONS, self::CDM_PART_DEFINITIONS_DEFAULT), JSON_PRETTY_PRINT);

    $is_custom_part_definition = $this->default_part_definition_json != $this->current_part_definition_json;
    $is_pre_380_part_definition = $default_part_definitions_pre_380_json == $this->current_part_definition_json;
    if($is_pre_380_part_definition){
      return self::PRE380_CONFIGURATION;
    } else if($is_custom_part_definition){
      return self::CUSTOM_CONFIGURATION;
    } else {
      return self::DEFAULT_CONFIGURATION;
    }
  }

  function nameRenderTemplateConfigurationStatus(){

    $default_render_templates_pre_380_json = json_encode(self::CDM_NAME_RENDER_TEMPLATES_DEFAULT_PRE_380, JSON_PRETTY_PRINT);
    $this->default_render_templates_json = json_encode(self::CDM_NAME_RENDER_TEMPLATES_DEFAULT, JSON_PRETTY_PRINT);
    $this->current_render_templates_json = json_encode(variable_get(self::CDM_NAME_RENDER_TEMPLATES, self::CDM_NAME_RENDER_TEMPLATES_DEFAULT), JSON_PRETTY_PRINT);
    $is_custom_render_template = $this->default_render_templates_json != $this->current_render_templates_json;
    $is_pre380_render_template = $default_render_templates_pre_380_json == $this->current_render_templates_json;
    if($is_pre380_render_template){
      return self::PRE380_CONFIGURATION;
    } else if($is_custom_render_template){
      return self::CUSTOM_CONFIGURATION;
    } else {
      return self::DEFAULT_CONFIGURATION;
    }
  }

  public function getDefaultPartDefinitionJson() {
    if($this->default_part_definition_json == null){
      // call the status function to initialize the fields
      $this->partDefinitionConfigurationStatus();
    }
    return $this->default_part_definition_json;
  }

  public function getCurrentPartDefinitionJson() {
    if($this->current_part_definition_json == null){
      // call the status function to initialize the fields
      $this->partDefinitionConfigurationStatus();
    }
    return $this->current_part_definition_json;
  }

  public function getCurrentRenderTemplatesJson() {
    if($this->current_render_templates_json == null){
      // call the status function to initialize the fields
      nameRenderTemplateConfigurationStatus();
    }
    return $this->current_render_templates_json;
  }

  public function getDefaultRenderTemplatesJson() {
    if($this->default_render_templates_json == null){
      // call the status function to initialize the fields
      nameRenderTemplateConfigurationStatus();
    }
    return $this->default_render_templates_json;
  }


}