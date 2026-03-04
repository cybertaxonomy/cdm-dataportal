/**
* Copyright (C) 2020 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.dataportal.elements;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.commons.lang3.StringUtils;
import org.openqa.selenium.WebElement;


/**
 * @author a.kohlbecker
 * @since May 26, 2020
 */
public class EntityType {

    private String cdmType;

    private static final Pattern pattern = Pattern.compile(".*(?:cdm\\:)([a-zA-Z]+).*");

    public static EntityType from(WebElement webElement) {
        String classAttributes = webElement.getAttribute("class");
        assert !StringUtils.isEmpty(classAttributes);
        Matcher m = pattern.matcher(classAttributes);
        assert m.matches();
        return new EntityType(m.group(1));
    }

    private EntityType(String cdmType) {
        this.cdmType = cdmType;
    }

    public String getCdmType() {
        return cdmType;
    }
}