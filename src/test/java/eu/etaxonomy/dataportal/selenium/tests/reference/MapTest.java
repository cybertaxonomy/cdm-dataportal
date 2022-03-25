/**
* Copyright (C) 2019 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.dataportal.selenium.tests.reference;

import java.io.UnsupportedEncodingException;
import java.net.MalformedURLException;
import java.util.List;
import java.util.UUID;

import org.junit.Before;
import org.junit.Test;

import eu.etaxonomy.dataportal.DataPortalSite;
import eu.etaxonomy.dataportal.elements.OpenLayersMap;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.NamePage;
import eu.etaxonomy.dataportal.pages.RegistrationPage;

/**
 * @author a.kohlbecker
 * @since Feb 5, 2019
 */
@DataPortalContexts( { DataPortalSite.reference })
public class MapTest extends CdmDataPortalTestBase {


    private static final String nodosilinea_radiophila_regid = "http://testbank.org/100004";

    private static final String ramsaria_regid = "http://testbank.org/100005";

    private static final UUID nodosilinea_radiophila_name_UUID = UUID.fromString("e97cc25b-ec11-4bb8-88d7-ab40a023f3fb");

    private static final UUID ramsaria_name_UUID = UUID.fromString("3a6d4bf2-5c89-4525-9e87-0bacac96990b");


    String titleSuffix = " | Integration test reference";



    @Before
    public void setUp() throws Exception {
        driver.get(getContext().getSiteUri().toString());
    }

    @Test
    public void namePageMapShown() throws MalformedURLException{

        NamePage p = new NamePage(driver, getContext(), nodosilinea_radiophila_name_UUID);
        List<OpenLayersMap> openLayersMaps = OpenLayersMap.findOpenLayersMaps(p);
        assertFalse(openLayersMaps.isEmpty());
        assertEquals(1, openLayersMaps.size());
        assertEquals("specimens", openLayersMaps.get(0).getMapName());
    }

    @Test
    public void registrationPageMapShown() throws MalformedURLException, UnsupportedEncodingException{

        RegistrationPage p = new RegistrationPage(driver, getContext(), nodosilinea_radiophila_regid);
        List<OpenLayersMap> openLayersMaps = OpenLayersMap.findOpenLayersMaps(p);
        assertFalse(openLayersMaps.isEmpty());
        assertEquals(1, openLayersMaps.size());
        assertEquals("specimens", openLayersMaps.get(0).getMapName());
    }

    @Test
    public void namePageMapHidden() throws MalformedURLException{

        NamePage p = new NamePage(driver, getContext(), ramsaria_name_UUID);
        List<OpenLayersMap> openLayersMaps = OpenLayersMap.findOpenLayersMaps(p);
        assertTrue(openLayersMaps.isEmpty());
    }

    @Test
    public void registrationPageMapHidden() throws MalformedURLException, UnsupportedEncodingException{

        RegistrationPage p = new RegistrationPage(driver, getContext(), ramsaria_regid);
        List<OpenLayersMap> openLayersMaps = OpenLayersMap.findOpenLayersMaps(p);
        assertTrue(openLayersMaps.isEmpty());
    }

}
