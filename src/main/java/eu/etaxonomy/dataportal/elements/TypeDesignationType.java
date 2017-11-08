/**
* Copyright (C) 2012 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.dataportal.elements;

import eu.etaxonomy.cdm.model.common.CdmBase;
import eu.etaxonomy.cdm.model.name.NameTypeDesignation;
import eu.etaxonomy.cdm.model.name.SpecimenTypeDesignation;

/**
 * @author andreas
 * @since Jul 30, 2012
 *
 */
public enum TypeDesignationType {

    nameTypeDesignation, specimenTypeDesignation;

    public static TypeDesignationType valueOfCdmClass(Class<? extends CdmBase> type){
        if(type.equals(SpecimenTypeDesignation.class)){
            return TypeDesignationType.specimenTypeDesignation;
        }
        if(type.equals(NameTypeDesignation.class)){
            return TypeDesignationType.nameTypeDesignation;
        }
        throw new IllegalArgumentException("no enum constant matching " + type.toString());
    }

}
