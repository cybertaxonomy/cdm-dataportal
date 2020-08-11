/**
* Copyright (C) 2020 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.dataportal.selenium.tests.reference;

import java.io.IOException;
import java.util.UUID;

import org.junit.Before;

import eu.etaxonomy.dataportal.DrupalVars;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;

/**
 * @author a.kohlbecker
 * @since Aug 11, 2020
 */
public class SpecimensTopDownViewTest extends CdmDataPortalTestBase {

    private static final UUID glenodinium_apiculatum_t = UUID.fromString("d245083e-3bda-435f-9bb3-bdc2249ff23c");

    @Before
    public void switchToView() throws IOException, InterruptedException {
        setDrupalVar(DrupalVars.CDM_DATAPORTAL_TAXONPAGE_TABS, "1");
    }

}
