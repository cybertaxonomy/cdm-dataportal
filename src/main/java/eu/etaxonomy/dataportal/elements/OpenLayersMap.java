/**
* Copyright (C) 2020 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.dataportal.elements;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

import org.apache.log4j.Logger;
import org.openqa.selenium.By;
import org.openqa.selenium.NoSuchElementException;
import org.openqa.selenium.WebElement;

import eu.etaxonomy.dataportal.pages.PortalPage;
import eu.etaxonomy.dataportal.selenium.UrlLoaded;

/**
 * @author a.kohlbecker
 * @since Jul 31, 2020
 */
public class OpenLayersMap {

    private WebElement webElement;
    private String mapId;
    private String mapName;

    private OpenLayersMap(WebElement webElement) {
        this.webElement = webElement;
        this.mapId = webElement.getAttribute("id");
        if (this.mapId != null) {
            this.mapName = webElement.getAttribute("id").replace("openlayers-container-", "");
        }
    }

    public static List<OpenLayersMap> findOpenLayersMaps(PortalPage page) {
        try {
            page.getWait().until(new UrlLoaded(page.getPageURL().toString()));
            List<WebElement> maps = page.getDataPortalContent().getElement()
                    .findElements(By.className("openlayers-container"));
            return maps.stream().map(m -> new OpenLayersMap(m)).collect(Collectors.toList());
        } catch (NoSuchElementException e) {
            Logger.getLogger(OpenLayersMap.class).info("No maps found", e);
            return new ArrayList<>();
        }
    }

    /**
     * @return the webElement
     */
    public WebElement getWebElement() {
        return webElement;
    }

    /**
     * @return the mapId
     */
    public String getMapId() {
        return mapId;
    }

    /**
     * @return the mapName
     */
    public String getMapName() {
        return mapName;
    }

}
