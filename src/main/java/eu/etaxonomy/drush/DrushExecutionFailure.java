/**
* Copyright (C) 2020 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.drush;

import java.util.List;
import java.util.stream.Collectors;

/**
 * @author a.kohlbecker
 * @since Aug 14, 2020
 */
public class DrushExecutionFailure extends Exception {

    private static final long serialVersionUID = -4840009591742936935L;

    String out, error, commandLineString;

    public DrushExecutionFailure(List<String> cmd, String out, String error) {
        this.out = out;
        this.error = error;
        this.commandLineString = cmd.stream().collect(Collectors.joining(" "));
    }

    @Override
    public String getMessage() {
        return "'" + commandLineString + "' failed with '" + error + "'";

    }

    @Override
    public String getLocalizedMessage() {
        return getMessage();

    }


}
