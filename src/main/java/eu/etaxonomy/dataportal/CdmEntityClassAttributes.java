/**
* Copyright (C) 2017 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.dataportal;

import java.util.Collection;
import java.util.UUID;

import org.apache.commons.lang3.StringUtils;

import eu.etaxonomy.cdm.model.CdmSimpleNameTypeFilter;
import eu.etaxonomy.cdm.model.CdmTypeScanner;
import eu.etaxonomy.cdm.model.common.CdmBase;

/**
 * Can parse class attributes created by the php function html_class_attribute_ref()
 * in modules/cdm_dataportal/cdm_dataportal.module
 *
 * Example:
 * <code>
 * cdm:SpecimenTypeDesignation uuid:35d876b0-8a13-44db-a66d-94acdd319c01
 * </code>
 *
 * @author a.kohlbecker
 * @since Oct 10, 2017
 *
 */
public class CdmEntityClassAttributes {

    private Class<? extends CdmBase> cdmType = null;

    private UUID enitytyUuid = null;

    public CdmEntityClassAttributes(String classAttributes) throws Exception{
        if(StringUtils.isNotEmpty(classAttributes)){
            for(String token : StringUtils.split(classAttributes)){
                if(token.startsWith("cdm:")){
                    setCdmType(findType(token.substring(4)));
                } else if(token.startsWith("uuid:")){
                    setEnitytyUuid(UUID.fromString(token.substring(5)));
                }
            }
        }
    }

    private Class<? extends CdmBase> findType(String simpleName) throws Exception {

        CdmTypeScanner<CdmBase> scanner = new CdmTypeScanner<CdmBase>(false, false);
        //scanner.addIncludeFilter(new AnnotationTypeFilter(Entity.class));
        scanner.addIncludeFilter(new CdmSimpleNameTypeFilter(simpleName));
        Collection<Class<? extends CdmBase>> classes = scanner.scanTypesIn("eu/etaxonomy/cdm/model");
        if(classes.isEmpty()){
            return null;
        } else if(classes.size() == 1){
            return classes.iterator().next();
        } else {
            throw new Exception("Multiple matching classes found for simple name '" + simpleName + "'");
        }

    }

    /**
     * @return the cdmType
     */
    public Class<? extends CdmBase> getCdmType() {
        return cdmType;
    }

    /**
     * @param cdmType the cdmType to set
     */
    public void setCdmType(Class<? extends CdmBase> cdmType) {
        this.cdmType = cdmType;
    }

    /**
     * @return the enitytyUuid
     */
    public UUID getEnitytyUuid() {
        return enitytyUuid;
    }

    /**
     * @param enitytyUuid the enitytyUuid to set
     */
    public void setEnitytyUuid(UUID enitytyUuid) {
        this.enitytyUuid = enitytyUuid;
    }

}
