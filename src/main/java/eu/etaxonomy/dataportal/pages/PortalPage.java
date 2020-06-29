package eu.etaxonomy.dataportal.pages;

import static org.junit.Assert.assertTrue;

import java.io.File;
import java.io.IOException;
import java.lang.reflect.Constructor;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.TimeUnit;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.stream.Collectors;

import org.apache.commons.io.FileUtils;
import org.apache.log4j.Logger;
import org.openqa.selenium.By;
import org.openqa.selenium.NoSuchElementException;
import org.openqa.selenium.OutputType;
import org.openqa.selenium.TakesScreenshot;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.interactions.Actions;
import org.openqa.selenium.support.CacheLookup;
import org.openqa.selenium.support.FindBy;
import org.openqa.selenium.support.FindBys;
import org.openqa.selenium.support.PageFactory;
import org.openqa.selenium.support.ui.WebDriverWait;

import com.google.common.base.Function;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.elements.BaseElement;
import eu.etaxonomy.dataportal.elements.ClassificationTreeBlock;
import eu.etaxonomy.dataportal.elements.LinkElement;
import eu.etaxonomy.dataportal.selenium.JUnitWebDriverWait;
import eu.etaxonomy.dataportal.selenium.UrlLoaded;

/**
 * FIXME only works with the cichorieae theme
 *
 * @author a.kohlbecker
 *
 */
public abstract class PortalPage {

    /**
     *
     */
    public static final int WAIT_SECONDS = 25;

    public static final Logger logger = Logger.getLogger(PortalPage.class);

    protected final static String DRUPAL_PAGE_QUERY = "q=";

    private final static Pattern DRUPAL_PAGE_QUERY_PATTERN = Pattern.compile("q=([^&]*)");

    public enum MessageType {status, warning, error} // needs to be lowercase

    protected WebDriver driver;

    protected DataPortalContext context;

    protected final JUnitWebDriverWait wait;

    public WebDriverWait getWait() {
        return wait;
    }


    /**
     * Implementations of this method will supply the relative
     * path to the Drupal page. This path will usually have the form
     * <code>cdm_dataportal/{nodetype}</code>. For example the taxon pages all
     * have the page base <code>cdm_dataportal/taxon</code>
     */
    protected abstract String getDrupalPageBase();

    private String initialDrupalPagePath;

    protected URL pageUrl;

    // ==== WebElements === //

    @FindBy(className="node")
    protected WebElement portalContent;

//    @FindBy(tagName="title")
//    @CacheLookup
//    protected WebElement title;

    @FindBy(className="node")
    protected WebElement node;

    @FindBys({@FindBy(id="tabs-wrapper"), @FindBy(className="primary")})
    @CacheLookup
    protected WebElement primaryTabs;

    @FindBy(id="block-cdm-dataportal-2")
    @CacheLookup
    protected WebElement searchBlockElement;

    @FindBy(id="block-cdm-taxontree-cdm-tree")
    @CacheLookup
    protected WebElement classificationBrowserBlock;

    @FindBy(className="messages")
    @CacheLookup
    protected List<WebElement> messages;

    /**
     * Creates a new PortaPage. Implementations of this class will provide the base path of the page by
     * implementing the method {@link #getDrupalPageBase()}. The constructor argument <code>pagePathSuffix</code>
     * specifies the specific page to navigate to. For example:
     * <ol>
     * <li>{@link #getDrupalPageBase()} returns <code>/cdm_dataportal/taxon</code></li>
     * <li><code>pagePathSuffix</code> gives <code>7fe8a8b6-b0ba-4869-90b3-177b76c1753f</code></li>
     * </ol>
     * Both are combined to form the URL pathelement <code>/cdm_dataportal/taxon/7fe8a8b6-b0ba-4869-90b3-177b76c1753f</code>
     *
     */
    public PortalPage(WebDriver driver, DataPortalContext context, String pagePathSuffix) throws MalformedURLException {

        this.driver = driver;

        this.context = context;

        this.wait = new JUnitWebDriverWait(driver, WAIT_SECONDS);

        this.initialDrupalPagePath = getDrupalPageBase() + (pagePathSuffix != null ? "/" + pagePathSuffix: "");

        this.pageUrl = new URL(context.getBaseUri().toString() + "?" + DRUPAL_PAGE_QUERY + initialDrupalPagePath);

        // tell browser to navigate to the page
        driver.get(pageUrl.toString());

        takeScreenShot();

        // This call sets the WebElement fields.
        PageFactory.initElements(driver, this);

        logger.info("loading " + pageUrl);

        try {
            String ignore_error = "Expecting web service to return pager objects but received an array";
            List<String> errors = getErrors().stream().filter(str -> str.startsWith(ignore_error)).collect(Collectors.toList());
            assertTrue("The page must not show an error box", errors.size() == 0);
        } catch (NoSuchElementException e) {
            //IGNORE since this is expected!
        }

    }

    /**
     * Creates a new PortaPage at given URL location. An Exception is thrown if
     * this URL is not matching the expected URL for the specific page type.
     *
     */
    public PortalPage(WebDriver driver, DataPortalContext context, URL url) throws Exception {

        this.driver = driver;

        this.context = context;

        this.wait = new JUnitWebDriverWait(driver, 25);

        this.pageUrl = new URL(context.getBaseUri().toString());

        // tell browser to navigate to the given URL
        driver.get(url.toString());

        takeScreenShot();

        if(!isOnPage()){
            throw new Exception("Not on the expected portal page ( current: " + driver.getCurrentUrl() + ", expected: " +  pageUrl + " )");
        }

        this.pageUrl = url;

        logger.info("loading " + pageUrl);

        // This call sets the WebElement fields.
        PageFactory.initElements(driver, this);

    }

    /**
     * Creates a new PortaPage at the WebDrivers current URL location. An Exception is thrown if
     * driver.getCurrentUrl() is not matching the expected URL for the specific page type.
     *
     */
    public PortalPage(WebDriver driver, DataPortalContext context) throws Exception {

        this.driver = driver;

        this.context = context;

        this.wait = new JUnitWebDriverWait(driver, 25);

        // preliminary set the pageUrl to the base path of this page, this is used in the next setp to check if the
        // driver.getCurrentUrl() is a sub path of the base path
        this.pageUrl = new URL(context.getBaseUri().toString());

        takeScreenShot();

        if(!isOnPage()){
            throw new Exception("Not on the expected portal page ( current: " + driver.getCurrentUrl() + ", expected: " +  pageUrl + " )");
        }

        // now set the real URL
        this.pageUrl = new URL(driver.getCurrentUrl());

        logger.info("loading " + pageUrl);

        // This call sets the WebElement fields.
        PageFactory.initElements(driver, this);

    }


    protected boolean isOnPage() {
        return driver.getCurrentUrl().startsWith(pageUrl.toString());
    }

    /**
     * navigate and reload the page if not jet there
     */
    public void get() {
        if(!driver.getCurrentUrl().equals(pageUrl.toString())){
            driver.get(pageUrl.toString());
            wait.until(new UrlLoaded(pageUrl.toString()));
            // take screenshot of new page
            takeScreenShot();
            PageFactory.initElements(driver, this);
        }
    }

    /**
     * go back in history
     */
    public void back() {
        driver.navigate().back();
    }

    public String getDrupalPagePath() {
        URL currentURL;
        try {
            currentURL = new URL(driver.getCurrentUrl());
            if(currentURL.getQuery() != null && currentURL.getQuery().contains(DRUPAL_PAGE_QUERY)){
                Matcher m = DRUPAL_PAGE_QUERY_PATTERN.matcher(currentURL.getQuery());
                m.matches();
                return m.group(1);
            } else {
                String uriBasePath = context.getBaseUri().getPath();
                if(!uriBasePath.endsWith("/")){
                    uriBasePath += "/";
                }
                return currentURL.getPath().replaceFirst("^"+ uriBasePath, "");
            }
        } catch (MalformedURLException e) {
            throw new RuntimeException(e);
        }
    }

    /**
     * Returns the the page path with which the page has initially been loaded.
     * Due to redirects the actual page path which can be retrieved by
     * {@link #getDrupalPagePath()} might be different
     *
     */
    public String getInitialDrupalPagePath() {
        return initialDrupalPagePath;
    }

    /**
     *
     * @return the page title
     * @deprecated use {@link WebDriver#getTitle()}
     */
    @Deprecated
    public String getTitle() {
        return driver.getTitle();
    }

    public List<String> getMessageItems(MessageType messageType){
        if(messages != null){
            for(WebElement m : messages){
                if(m.getAttribute("class").contains(messageType.name())){
                    List<WebElement> messageItems = m.findElements(By.cssSelector("ul.messages__list li"));
                    if(messageItems.size() == 0 && !m.getText().isEmpty()) {
                        // we have only one item which is not shown as list.
                        messageItems.add(m);

                    }
                    return messageItems.stream().map(mi -> mi.getText()).collect(Collectors.toList());
                }
            }
        }
        return new ArrayList<>();
    }

    /**
     * @return the warning messages from the Drupal message box
     */
    public List<String> getWarnings() {
        return getMessageItems(MessageType.warning);
    }

    /**
     * @return the error messages from the Drupal message box
     */
    public List<String> getErrors() {
        return getMessageItems(MessageType.error);
    }

    public String getAuthorInformationText() {

        WebElement authorInformation = null;

        try {
            authorInformation  = node.findElement(By.className("submitted"));
        } catch (NoSuchElementException e) {
            // IGNORE //
        }


        if(authorInformation != null){
            return authorInformation.getText();
        } else {
            return null;
        }
    }

    public List<LinkElement> getPrimaryTabs(){
        List<LinkElement> tabs = new ArrayList<LinkElement>();
        List<WebElement> links = primaryTabs.findElements(By.tagName("a"));
        for(WebElement a : links) {
            WebElement renderedLink = a;
            if(renderedLink.isDisplayed()){
                tabs.add(new LinkElement(renderedLink));
            }
        }

        return tabs;
    }

    public ClassificationTreeBlock getClassificationTree() {
        return new ClassificationTreeBlock(classificationBrowserBlock);
    }

    public void hover(WebElement element) {
        Actions actions = new Actions(driver);
        actions.moveToElement(element, 1, 1).perform();
        logger.debug("hovering");
    }


    /**
     * @return the current URL string from the {@link WebDriver}
     */
    public URL getPageURL() {
        return pageUrl;
    }


    /**
     * @return the <code>scheme://domain:port</code> part of the initial url of this page.
     */
    public String getInitialUrlBase() {
        return pageUrl.getProtocol() + "://" + pageUrl.getHost() + pageUrl.getPort();
    }

    @Override
    public boolean equals(Object obj) {
        if (PortalPage.class.isAssignableFrom(obj.getClass())) {
            PortalPage page = (PortalPage) obj;
            return this.getPageURL().toString().equals(page.getPageURL().toString());

        } else {
            return false;
        }
    }


    /**
     * @param isTrue see {@link org.openqa.selenium.support.ui.FluentWait#until(Function)}
     * @param pageType the return type
     * @param duration may be null, if this in null <code>waitUnit</code> will be ignored.
     * @param waitUnit may be null, is ignored if <code>duration</code> is null defaults to {@link TimeUnit#SECONDS}

     */
    public <T extends PortalPage> T clickLink(BaseElement element, Function<? super WebDriver, Boolean> isTrue, Class<T> pageType, Long duration, TimeUnit waitUnit) {

        if( pageType.getClass().equals(PortalPage.class) ) {
            throw new RuntimeException("Parameter pageType must be a subclass of PortalPage");
        }
        String targetWindow = null;
        List<String> targets = element.getLinkTargets(driver);
        if(targets.size() > 0){
            targetWindow = targets.get(0);
        }

        if(logger.isInfoEnabled() || logger.isDebugEnabled()){
            logger.info("clickLink() on " + element.toStringWithLinks());
        }
        element.getElement().click();
        if(targetWindow != null){
            driver.switchTo().window(targetWindow);
        }

        try {
            if(duration != null){
                if(waitUnit == null){
                    waitUnit = TimeUnit.SECONDS;
                }
                wait.withTimeout(duration, waitUnit).until(isTrue);
            } else {
                wait.until(isTrue);
            }
        } catch (AssertionError timeout){
            logger.info("clickLink timed out. Current WindowHandles:" + driver.getWindowHandles());
            throw timeout;
        }


        Constructor<T> constructor;
        T pageInstance;
        try {
            constructor = pageType.getConstructor(WebDriver.class, DataPortalContext.class);
            pageInstance = constructor.newInstance(driver, context);
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
        // take screenshot of new page
        takeScreenShot();
        return pageInstance;
    }


    /**
     * @param isTrue see {@link org.openqa.selenium.support.ui.FluentWait#until(Function)}
     * @param type the return type

     */
    public <T extends PortalPage> T clickLink(BaseElement element, Function<? super WebDriver, Boolean> isTrue, Class<T> type) {
        return clickLink(element, isTrue, type, null, null);
    }


    /**
     * replaces all underscores '_' by hyphens '-'
     */
    protected String normalizeClassAttribute(String featureName) {
        return featureName.replace('_', '-');
    }

    public File takeScreenShot(){


        logger.info("Screenshot ...");
        File destFile = fileForTestMethod(new File("screenshots"));
        if(logger.isDebugEnabled()){
            logger.debug("Screenshot destFile" + destFile.getPath().toString());
        }
        File scrFile = ((TakesScreenshot)driver).getScreenshotAs(OutputType.FILE);
        try {
            FileUtils.copyFile(scrFile, destFile);
            logger.info("Screenshot taken and saved as " + destFile.getAbsolutePath());
            return destFile;
        } catch (IOException e) {
            logger.error("could not copy sceenshot to " + destFile.getAbsolutePath(), e);
        }

        return null;

    }

    /**
     * Finds the test class and method in the stack trace which is using the
     * PortalPage and returns a File object consisting of:
     * {@code $targetFolder/$className/$methodName }
     *
     *
     * If no test class is found it will fall back to using
     * "noTest" as folder name and a timestamp as filename.
     */
    private File fileForTestMethod(File targetFolder){

        StackTraceElement[] trace = Thread.currentThread().getStackTrace();
        for (StackTraceElement stackTraceElement : trace) {
            // according to the convention all test class names should end with "Test"
            if(logger.isTraceEnabled()){
                logger.trace("fileForTestMethod() - " + stackTraceElement.toString());
            }
            if(stackTraceElement.getClassName().endsWith("Test")){
                return uniqueIndexedFile(
                            targetFolder.getAbsolutePath() + File.separator + stackTraceElement.getClassName(),
                            stackTraceElement.getMethodName(),
                            "png");

            }
        }
        return uniqueIndexedFile(
                targetFolder.getAbsolutePath() + File.separator + "noTest",
                Long.toString(System.currentTimeMillis()),
                "png");
    }


    private File uniqueIndexedFile(String folder, String fileName, String suffix){
        File file;
        int i = 0;
        while(true){
            file = new File(folder + File.separator + fileName + "_" + Integer.toString(i++) + "." + suffix);
            if(!file.exists()){
                return file;
            }
        }
    }


}
