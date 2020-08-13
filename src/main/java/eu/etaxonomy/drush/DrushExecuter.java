/**
* Copyright (C) 2020 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.drush;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.net.URI;
import java.net.URISyntaxException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.commons.io.IOUtils;
import org.apache.commons.lang3.SystemUtils;
import org.apache.log4j.Level;
import org.apache.log4j.Logger;

import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;

/**
 * Java executor for drush (https://www.drush.org/).
 *
 * <h3>Local usage:</h3> Set the {@link #setDrupalRoot(File) drupal root folder}
 * and the {@link #setSiteURI(URI) site uri}.
 *
 * <h3>Remote usage:</h3> In addition to the above properties you can also
 * define an {@link #setSshHost(String) ssh host} and optionally the according
 * {@link #setSshUser(String) ssh user} to execute drush on a remote machine.
 *
 * @author a.kohlbecker
 * @since Jul 31, 2020
 */
public class DrushExecuter {

    public static final Logger logger = Logger.getLogger(DrushExecuter.class);

    private URI siteURI = null;

    private File drupalRoot = null;

    private String sshUser = null;

    private String sshHost = null;

    public DrushExecuter() throws IOException, InterruptedException {
        findDrushCommand();
    }

    /**
     * The execution of this command via
     * <code>DrushExecuter.execute({@linkplain DrushCommand#version})</code> results in
     * a {@code List<String>} return variable with the following elements:
     *
     * <ol>
     * <li>major</li>
     * <li>minor</li>
     * <li>patch</li>
     * <ol>
     */
    public static DrushCommand version = new DrushCommand(Arrays.asList("--version"),
            "Drush Version\\s+:\\s+(?<major>\\d+)\\.(?<minor>\\d+)\\.(?<patch>\\d+)", null);

    public static DrushCommand help = new DrushCommand(Arrays.asList("help"), null, null);

    public static DrushCommand coreStatus = new DrushCommand(Arrays.asList("core-status"), null, null);

    /**
     * Executes {@code drush vget --exact <variable-key>}
     * <p>
     * The execution of this command via
     * <code>DrushExecuter.execute({@linkplain DrushCommand#variableSet})</code> results in
     * a {@code List<String>} return variable with the following elements:
     *
     * <ol>
     * <li>value</li>
     * <li>value</li>
     * <li>value</li>
     * <ol>
     */
    public static DrushCommand variableGet = new DrushCommand(Arrays.asList("vget", "--exact", "--format=json", "%s"));
    /**
     * Executes {@code drush vset --exact <variable-key> <variable-value>}
     * <p>
     * The execution of this command via
     * <code>DrushExecuter.execute({@linkplain DrushCommand#variableSet})</code> results in
     * a {@code List<String>} return variable with the following elements:
     *
     * <ol>
     * <li>value</li>
     * <li>status</li>
     * <ol>
     */
    public static DrushCommand variableSet = new DrushCommand(Arrays.asList("--yes", "vset", "%s", "%s"), null,
            "[^\\\"]*\\\"(.*)\\\".*\\[(\\w+)\\]"
            );

    /**
     * @throws IOException
     *             if an I/O error occurs in the ProcessBuilder
     * @throws InterruptedException
     *             if the Process was interrupted
     */
    private void findDrushCommand() throws IOException, InterruptedException {

        if (SystemUtils.IS_OS_WINDOWS) {
            throw new RuntimeException("not yet implmented for Windows");
        }

        List<Object> matches = execute(version);
        assert !((String) matches.get(0)).isEmpty() : "No suitable drush command found in the system";
        String majorVersion = (String) matches.get(0);
        if (Integer.valueOf(majorVersion) < 8) {
            throw new RuntimeException("drush version >= 8 required");
        }

    }

    /**
     * @throws IOException
     *             if an I/O error occurs in the ProcessBuilder
     * @throws InterruptedException
     *             if the Process was interrupted
     */
    public List<Object> execute(DrushCommand cmd, String... value) throws IOException, InterruptedException {

        List<String> executableWithArgs = new ArrayList<>();

        if (sshHost != null) {
            executableWithArgs.add("ssh");
            String userHostArg = sshHost;
            if (sshUser != null) {
                userHostArg = sshUser + "@" + sshHost;
            }
            executableWithArgs.add(userHostArg);
        }

        executableWithArgs.add("drush");

        if (drupalRoot != null) {
            executableWithArgs.add("--root=" + drupalRoot.toString());
        }
        if (siteURI != null) {
            executableWithArgs.add("--uri=" + siteURI.toString());
        }
        int commandSubstitutions = 0;
        for (String arg : cmd.args) {
            if (arg.contains("%s")) {
                executableWithArgs.add(String.format(arg, value[commandSubstitutions]));
                commandSubstitutions++;
            } else {
                executableWithArgs.add(arg);
            }
        }

        List<Object> matches = new ArrayList<>();

        ProcessBuilder pb = new ProcessBuilder(executableWithArgs);
        logger.debug("Command: " + pb.command().toString());
        Process process = pb.start();
        int exitCode = process.waitFor();

        if (exitCode == 0) {
            String out, error;
            if(cmd.jsonResult) {
                out = readExecutionResponse(matches, process.getInputStream());
                error = readExecutionResponse(matches, process.getErrorStream());
            } else {
                out = readExecutionResponse(matches, process.getInputStream(), cmd.outRegex);
                error = readExecutionResponse(matches, process.getErrorStream(), cmd.errRegex);
            }
            if (out != null && !out.isEmpty()) {
                logger.error(error);
            }
            if (error != null && !error.isEmpty()) {
                logger.error(error);
            }
        } else {
            throw new RuntimeException(IOUtils.toString(process.getErrorStream()));
        }
        return matches;
    }

    protected String readExecutionResponse(List<Object> matches, InputStream stream, Pattern regex) throws IOException {
        String out;
        if (regex != null) {
            Scanner scanner = new Scanner(stream);
            while (true) {
                out = scanner.findWithinHorizon(regex, 0);
                if (out == null) {
                    break;
                }
                if (out != null) {
                    Matcher m = regex.matcher(out);
                    int patternMatchCount = 0;
                    while (m.find()) {
                        patternMatchCount++;
                        if (m.groupCount() > 0) {
                            for (int g = 1; g <= m.groupCount(); g++) {
                                matches.add(m.group(g));
                                logger.debug("match[" + patternMatchCount + "." + g + "]: " + m.group(g));
                            }
                        } else {
                            matches.add(m.group(0));
                            logger.debug("entire pattern match[" + patternMatchCount + ".0]: " + m.group(0));
                        }
                    }
                }
            }
            scanner.close();
            return null;
        } else {
            out = IOUtils.toString(stream);
            logger.debug(out);
            return out;
        }
    }

    /**
     * @param matches
     * @param stream
     * @return depending on the drupal variable type different return types are possible:
     *  <ul>
     *  <li>Object</li>
     *  <li>List</li>
     *  <li>List</li>
     *  <li>String</li>
     *  <li>Double</li>
     *  <li>Integer</li>
     *  </ul>
     *
     * @throws IOException
     */
    protected String readExecutionResponse(List<Object> matches, InputStream stream) throws IOException {
        String out = IOUtils.toString(stream);
        if(out != null) {
            out = out.trim();
            if(!out.isEmpty()) {
                ObjectMapper mapper = new ObjectMapper();
                if(out.startsWith("[")) {
                   matches.add(mapper.readValue(out, new TypeReference<List<Object>>(){}));
                } else  {
                   matches.add(mapper.readValue(out, Object.class));
                }
                if(matches.isEmpty()) {
                    logger.debug("no result");
                } else {
                    logger.debug("result object: " + matches.get(0));
                }
            }

        }
        return out;
    }

    public static class DrushCommand {

        Pattern outRegex;
        Pattern errRegex;
        boolean jsonResult = false;
        List<String> args = new ArrayList<>();

        public DrushCommand(List<String> args, String outRegex, String errRegex) {
            this.args = args;
            if (outRegex != null) {
                this.outRegex = Pattern.compile(outRegex, Pattern.MULTILINE);
            }
            if (errRegex != null) {
                this.errRegex = Pattern.compile(errRegex, Pattern.MULTILINE);
            }
        }

        /**
         * For drush commands suopporting the {@code --format=json} option.
         * @param args
         */
        public DrushCommand(List<String> args) {
            this.args = args;
            this.jsonResult = true;
        }
    }

    /**
     * These tests have not been implemented as jUnit tests since the execution
     * it too much dependent from the local environment. Once the
     * <code>DrushExecuter</code> is being used in the selenium test suite will
     * be tested implicitly anyway.
     */
    public static void main(String[] args) throws URISyntaxException {
        DrushExecuter.logger.setLevel(Level.DEBUG);
        try {
            DrushExecuter dex = new DrushExecuter();
            List<Object> results;
            dex.setDrupalRoot(new File("/home/andreas/workspaces/www/drupal-7"));
            dex.setSiteURI(new URI("http://edit.test/d7/caryophyllales/"));
//            dex.execute(coreStatus);
//            dex.execute(help);
            results = dex.execute(variableSet, "cdm_webservice_url",
                    "http://api.cybertaxonomy.org/cyprus/");
            if (!results.get(0).equals("http://api.cybertaxonomy.org/cyprus/")) {
                throw new RuntimeException("unexpected result item 0: " + results.get(0));
            }
            if (!results.get(1).equals("success")) {
                throw new RuntimeException("unexpected result item 1: " + results.get(0));
            }
            results = dex.execute(variableGet, "cdm_webservice_url");
            if (!results.get(0).equals("http://api.cybertaxonomy.org/cyprus/")) {
                throw new RuntimeException("unexpected result item 0: " + results.get(0));
            }
            // testing remote execution via ssh
            dex.sshHost = "edit-int";
            dex.setDrupalRoot(new File("/var/www/drupal-7"));
            dex.setSiteURI(new URI("http://int.e-taxonomy.eu/dataportal/integration/cyprus"));
            results = dex.execute(variableGet, "cdm_webservice_url");
            if (!results.get(0).equals("http://int.e-taxonomy.eu/cdmserver/integration_cyprus/")) {
                throw new RuntimeException("unexpected result item 0: " + results.get(0));
            }

        } catch (IOException | InterruptedException | AssertionError e) {
            e.printStackTrace();
        }
    }

    public URI getSiteURI() {
        return siteURI;
    }

    public void setSiteURI(URI siteURI) {
        this.siteURI = siteURI;
    }

    public File getDrupalRoot() {
        return drupalRoot;
    }

    public void setDrupalRoot(File drupalRoot) {
        this.drupalRoot = drupalRoot;
    }

    public String getSshUser() {
        return sshUser;
    }

    public void setSshUser(String sshUser) {
        this.sshUser = sshUser;
    }

    public String getSshHost() {
        return sshHost;
    }

    public void setSshHost(String sshHost) {
        this.sshHost = sshHost;
    }
}
