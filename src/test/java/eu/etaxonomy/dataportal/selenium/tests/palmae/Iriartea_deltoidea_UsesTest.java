/**
 * Copyright (C) 2009 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 *
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */
package eu.etaxonomy.dataportal.selenium.tests.palmae;

import java.net.MalformedURLException;
import java.util.List;
import java.util.UUID;

import org.junit.Before;
import org.junit.Test;
import org.openqa.selenium.By;
import org.openqa.selenium.WebElement;

import eu.etaxonomy.dataportal.DataPortalSite;
import eu.etaxonomy.dataportal.elements.FeatureBlock;
import eu.etaxonomy.dataportal.elements.LinkElement;
import eu.etaxonomy.dataportal.junit.CdmDataPortalTestBase;
import eu.etaxonomy.dataportal.junit.DataPortalContextSuite.DataPortalContexts;
import eu.etaxonomy.dataportal.pages.TaxonProfilePage;

/**
 * @author a.kohlbecker
 *
 */

@DataPortalContexts( { DataPortalSite.palmae })
public class Iriartea_deltoidea_UsesTest extends CdmDataPortalTestBase{

    static UUID taxonUuid = UUID.fromString("ce870eca-0422-4a3c-b849-0f5ca0370b1f");

    static TaxonProfilePage p = null;

    @Before
    public void setUp() throws MalformedURLException {

        if(p == null){
            p = new TaxonProfilePage(driver, getContext(), taxonUuid);
        }

    }


    @Test
    public void testTitleAndTabs() {

        assertEquals(getContext().prepareTitle("Iriartea deltoidea Ruiz & Pav., Syst. Veg. Fl. Peruv. Chil. : 298 (1798)"), driver.getTitle());
        assertNull("Authorship information should be hidden", p.getAuthorInformationText());

        List<LinkElement> primaryTabs = p.getPrimaryTabs();
        int tabId = 0;
        assertEquals("General\n(active tab)", primaryTabs.get(tabId++).getText());
        assertEquals("Synonymy", primaryTabs.get(tabId++).getText());
        assertEquals("Images", primaryTabs.get(tabId++).getText());
        assertEquals("Expecting " + tabId + " tabs", tabId, primaryTabs.size());

    }


    @Test
    public void testFeatureToc() {

        assertEquals("Content", p.getTableOfContentHeader());
        List<LinkElement> links = p.getTableOfContentLinks();
        assertEquals("Expecting 7 entries in the TOC of the profile page.", 7, links.size());
    }

    @Test
    public void testFeatureDistribution() {

        int featureId = 0;

        int descriptionElementFontSize = 11;
        String expectedListStyleType = "none";
        String expectedCssDisplay = "inline";
        String expectedListStylePosition = "outside";
        String expectedListStyleImage = "none";
        int indent = 23;

        /* distribution */
        String featureClass = "distribution";
        String featureLabel = "Distribution";
        String blockTextFull = "Map uses TDWG level 3 distributions (http://www.nhm.ac.uk/hosted_sites/tdwg/geogrphy.html)\n"
                + "Bolivia (World Checklist of Arecaceae), Brazil North (World Checklist of Arecaceae), Colombia (World Checklist of Arecaceae), Costa Rica (World Checklist of Arecaceae), Ecuador (World Checklist of Arecaceae), Nicaragua (World Checklist of Arecaceae), Panamá (World Checklist of Arecaceae), Peru (World Checklist of Arecaceae), Venezuela (World Checklist of Arecaceae)\n"
                + "Central America to Ecuador W of the Andes, and in the W part of the Amazon region from Venezuela to Bolivia. Perhaps the most common native tree species in Ecuador, occurring in all provinces that include moist lowland areas. (Borchsenius F., Borgtoft-Pedersen H. and Baslev H. 1998. Manual to the Palms of Ecuador. AAU Reports 37. Department of Systematic Botany, University of Aarhus, Denmark in collaboration with Pontificia Universidad Catalica del Ecuador)";

        p.testTableOfContentEntry(featureId, featureLabel, featureClass);
        FeatureBlock featureBlockDistribution = p.getFeatureBlockAt(featureId, featureClass, "div", "span");

        assertEquals(featureLabel, featureBlockDistribution.getTitle().getText());
        String distributionContentText = featureBlockDistribution.getContentText().trim();
        assertEquals(blockTextFull, distributionContentText);

        featureBlockDistribution.testDescriptionElementLayout(0, indent, descriptionElementFontSize, expectedCssDisplay, expectedListStyleType, expectedListStylePosition, expectedListStyleImage);

        assertNotNull("Expecting an OpenLayers map", featureBlockDistribution.getElement().findElement(By.id("openlayers-map-distribution")));
        String mapText = featureBlockDistribution.getElement().findElement(By.className("distribution_map_caption")).getText();
        assertTrue(mapText.equals("Map uses TDWG level 3 distributions (http://www.nhm.ac.uk/hosted_sites/tdwg/geogrphy.html)")
                // OR error text which is shown when the map server is absent
                || mapText.equals("The map is currently broken due to problems with the map server."));
    }

    @Test
    public void testUses() {

        int featureId = 2;
        int descriptionElementFontSize = 11;
        String expectedCssDisplay = "inline";
        String expectedListStyleType = "none";
        String expectedListStylePosition = "outside";
        String expectedListStyleImage = "none";
        int indent = 23;

        /* Biology And Ecology */
        String featureClass = "uses";
        String featureLabel = "Uses";
        String blockTextFull = featureLabel + "\nThe outer part of the stems are used throughout its range for building purposes, e.g., floors, posts, poles; also for blowguns, bows, harpoons and arrow points; and also for firewood. The leaves are used for thatching and basketry. The heart and seeds are occasionally eaten. The inside layer of the leaf sheath is used to give women strength in labor (Shemluck & Ness 163, Ecuador). Hollowed-out stems are used as coffins by Embera Indians in Colombia (R. Bernal, pers. comm.). Steven King (pers. comm.) reports that in northern Peru Angotere-Secoya and Quechua people use the stems of I. deltoidea as canoes. Large specimens are selected and carefully felled. The soft central ground tissue is removed from the center of the stem, and base and apex fashioned into bow and stern. The canoes are widely used for shortening trips, especially long overland trips where short-cuts can be made by river. Canoes last about two or three months. Such is the demand for these temporary canoes that many of the larger specimens of Iriartea have been felled in this area. Rodrigo Bernal (pers. comm.) reports that in Colombia the Embera Indians of the Choco tie the stems together and use them as rafts. Since these are so heavy they are only used for downstream travel. (Henderson, A. 1990. Introduction and the Iriarteinae. Flora Neotropica Monograph 53.)";
        expectedCssDisplay = "list-item";
        expectedListStyleType = "none";

        p.testTableOfContentEntry(featureId,featureLabel, featureClass);
        FeatureBlock featureBlockBibliography = p.getFeatureBlockAt(featureId, featureClass, "ul", "li");

        assertEquals(blockTextFull, featureBlockBibliography.getText());
        featureBlockBibliography.testDescriptionElementLayout(0, indent, descriptionElementFontSize, expectedCssDisplay, expectedListStyleType, expectedListStylePosition, expectedListStyleImage);

    }

    @Test
    public void testFeatureUseRecord() {

        int featureId = 5;

        String featureClass = "use-record";
        String featureLabel = "Use Record";

        p.testTableOfContentEntry(featureId,featureLabel, featureClass);
        FeatureBlock featureBlockBioEco = p.getFeatureBlockAt(featureId, featureClass, "ul", "li");

        List<WebElement> listItems = featureBlockBioEco.getElement().findElements(By.tagName("li"));
        assertEquals(95, listItems.size());

        String item14UseRecordText =
                "Use Category Use Sub Category Plant Part Human Group Ethnic Group Country\n"
                + "Construction Thatch Entire leaf Indigenous Tsimane/Mosetene Bolivia\n"
                + "Utensils and Tools Domestic Stem Indigenous Tsimane/Mosetene Bolivia\n"
                + "Utensils and Tools Domestic Leaf sheath Indigenous Tsimane/Mosetene Bolivia\n"
                + "Human Food Food Palm heart Indigenous Tsimane/Mosetene Bolivia\n"
                + "Cultural Personal adornment Seeds Indigenous Tsimane/Mosetene Bolivia\n"
                + "Construction Houses Stem Indigenous Tsimane/Mosetene Bolivia";


        assertEquals("Iriartea deltoidea Ruiz & Pav.: Alimentación humana. Palmito. Cuando es tierno, se corta y pela para consumir crudo en ensalada. Comercial. Semilla. Es utilizada en la elaboración de collares. Construcción. Estípite. Construcción de viviendas, en postes y vigas transversales. La parte externa es cortada el segmentos longitudinales, secada, y empleada en las paredes, suelo, y como cuerda natural. Construcción. Hoja. Techado de campamentos temporales. Utensilios y herr. de uso doméstico. Estípite. Elaboración de \"guarachas\" (estantes para depositar objetos). Utensilios y herr. de uso doméstico. Hoja (vaina). La vaina seca de la hoja se utiliza para hacer recipientes. (Armesilla, P.J., Usos de las palmeras (Arecaceae),en la Reserva de la Biosfera-Tierra Comunitaria de Orígen Pilón Lajas, (Bolivia). 2006)\n"
                + item14UseRecordText,
                listItems.get(0).getText());
        // tabular
        assertEquals(item14UseRecordText,
                listItems.get(0).findElement(By.className("use-records")).getText());
        // synonym
        assertEquals("Iriartea ventricosa Mart.: De las líneas de la hoja de esta palmera que crece en forma silvestre, los tacana elaboran recipientes de diferentes tamaños. (…). Para la elaboración de cercos , (…), los tacana utilizan los troncos de las palmeras chonta, motacú, copa, assai, majillo, pachiuva, tola y tuana. (…). Para la elaboración de cercos , (…), los tacana utilizan los troncos de las palmeras chonta, motacú, copa, assai, majillo, pachiuva, tola y tuana. (Hissink, K., and A. Hahn, Los Tacana- datos sobre la historia de su civilización. 2000 (as Iriartea ventricosa Mart.))",
            listItems.get(81).getText());

    }

    @Test
    public void testBibliography() {

        int featureId = 6;
        int descriptionElementFontSize = 11;
        String expectedCssDisplay = "block";
        String expectedListStyleType = "none";
        String expectedListStylePosition = "outside";
        String expectedListStyleImage = "none";
        int indent = 0;

        String featureClass = "bibliography";
        String featureLabel = "Bibliography";
        String blockTextFull = featureLabel + "\n"
                + " Borchsenius F., Borgtoft-Pedersen H. and Baslev H. 1998. Manual to the Palms of Ecuador. AAU Reports 37. Department of Systematic Botany, University of Aarhus, Denmark in collaboration with Pontificia Universidad Catalica del Ecuador\n"
                + " World Checklist of Arecaceae\n"
                + " Henderson, A. 1990. Introduction and the Iriarteinae. Flora Neotropica Monograph 53.";

        p.testTableOfContentEntry(featureId,featureLabel, featureClass);
        FeatureBlock featureBlockBibliography = p.getFeatureBlockAt(featureId, featureClass, "ul", "div");

        // assertEquals(blockTextFull, featureBlockBibliography.getText());
        // featureBlockBibliography.testDescriptionElementLayout(0, indent, descriptionElementFontSize, expectedCssDisplay, expectedListStyleType, expectedListStylePosition, expectedListStyleImage);

    }

}
