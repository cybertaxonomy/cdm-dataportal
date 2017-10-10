/**
* Copyright (C) 2017 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.dataportal;

import org.junit.Assert;
import org.junit.Test;

import eu.etaxonomy.cdm.model.name.SpecimenTypeDesignation;

/**
 * @author a.kohlbecker
 * @since Oct 10, 2017
 *
 */
public class CdmEntityClassAttributesTest extends Assert {

    @Test
    public void testTypeName() throws Exception {
        CdmEntityClassAttributes a = new CdmEntityClassAttributes("cdm:SpecimenTypeDesignation");
        assertEquals(SpecimenTypeDesignation.class, a.getCdmType());
        assertNull(a.getEnitytyUuid());
    }

    @Test
    public void testUuid() throws Exception {
        CdmEntityClassAttributes a = new CdmEntityClassAttributes("uuid:a2f7451b-bd67-4265-8b21-94943503c93f");
        assertEquals("a2f7451b-bd67-4265-8b21-94943503c93f", a.getEnitytyUuid().toString());
        assertNull(a.getCdmType());
    }

    @Test
    public void testUuidAndTypeName() throws Exception {
        CdmEntityClassAttributes a = new CdmEntityClassAttributes("someAttribute cdm:SpecimenTypeDesignation anotherAttribute uuid:a2f7451b-bd67-4265-8b21-94943503c93f lastAttribute");
        assertEquals(SpecimenTypeDesignation.class, a.getCdmType());
        assertEquals("a2f7451b-bd67-4265-8b21-94943503c93f", a.getEnitytyUuid().toString());
    }

    @Test
    public void testInterfaceName() throws Exception {
        CdmEntityClassAttributes a = new CdmEntityClassAttributes("cdm:ITypeDesignation");
        assertNull(a.getCdmType());
        assertNull(a.getEnitytyUuid());
    }

    @Test
    public void testEmpty() throws Exception {
        CdmEntityClassAttributes a = new CdmEntityClassAttributes("someAttribute anotherAttribute lastAttribute");
        assertNull(a.getCdmType());
        assertNull(a.getEnitytyUuid());
    }

}
