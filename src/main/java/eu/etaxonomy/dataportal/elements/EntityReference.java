/**
* Copyright (C) 2020 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.dataportal.elements;

import java.util.UUID;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.commons.lang3.StringUtils;
import org.openqa.selenium.WebElement;


/**
 * @author a.kohlbecker
 * @since May 26, 2020
 */
public class EntityReference {

    /**
     * @return the cdmType
     */
    public String getCdmType() {
        return cdmType;
    }

    /**
     * @return the uuid
     */
    public UUID getUuid() {
        return uuid;
    }

    String cdmType;
    UUID uuid;

    private static final Pattern pattern = Pattern.compile(".*(?:cdm\\:)([a-zA-Z]+).*(?:uuid\\:)([0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}).*");


    private EntityReference(String cdmType, UUID uuid) {
        this.cdmType = cdmType;
        this.uuid = uuid;
    }

    public static EntityReference from(WebElement webElement) {
        String classAttributes = webElement.getAttribute("class");
        assert !StringUtils.isEmpty(classAttributes);
        Matcher m = pattern.matcher(classAttributes);
        assert m.matches();
        return new EntityReference(m.group(1), UUID.fromString(m.group(2)));
    }

}
