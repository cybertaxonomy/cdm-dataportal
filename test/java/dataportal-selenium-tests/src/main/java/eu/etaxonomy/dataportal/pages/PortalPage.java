package eu.etaxonomy.dataportal.pages;

import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;

import org.apache.log4j.Logger;
import org.openqa.selenium.By;
import org.openqa.selenium.NoSuchElementException;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.interactions.Actions;
import org.openqa.selenium.support.CacheLookup;
import org.openqa.selenium.support.FindBy;
import org.openqa.selenium.support.FindBys;
import org.openqa.selenium.support.PageFactory;
import org.openqa.selenium.support.ui.WebDriverWait;

import eu.etaxonomy.dataportal.DataPortalContext;
import eu.etaxonomy.dataportal.elements.LinkElement;
import eu.etaxonomy.dataportal.selenium.JUnitWebDriverWait;

public abstract class  PortalPage {

	/**
	 *
	 */
	public static final int WAIT_SECONDS = 25;

	public static final Logger logger = Logger.getLogger(PortalPage.class);

	protected final static String DRUPAL_PAGE_QUERY_BASE = "?q=";

	protected WebDriver driver;

	protected DataPortalContext context;

	protected final JUnitWebDriverWait wait;

	public WebDriverWait getWait() {
		return wait;
	}


	/**
	 * Implementations of this method will supply the relative
	 * path to the Drupal page. This path will usally have the form
	 * <code>cdm_dataportal/{nodetype}</code>. For example the taxon pages all
	 * have the page base <code>cdm_dataportal/taxon</code>
	 * @return
	 */
	protected abstract String getDrupalPageBase();

	private String drupalPagePath;

	protected URL pageUrl;

	// ==== WebElements === //

	@FindBy(id="cdm_dataportal.node")
	protected WebElement portalContent;

	@FindBy(tagName="title")
	@CacheLookup
	protected WebElement title;

	@FindBy(className="node")
	protected WebElement node;

	@FindBys({@FindBy(id="tabs-wrapper"), @FindBy(className="primary")})
	@CacheLookup
	protected WebElement primaryTabs;

	@FindBy(id="block-cdm_dataportal-2")
	@CacheLookup
	protected WebElement searchBlockElement;

	@FindBy(id="block-cdm_taxontree-cdm_tree")
	@CacheLookup
	protected WebElement classificationBrowserBlock;

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
	 *
	 * @param driver
	 * @param context
	 * @param pagePathSuffix
	 * @throws MalformedURLException
	 */
	public PortalPage(WebDriver driver, DataPortalContext context, String pagePathSuffix) throws MalformedURLException {

		this.driver = driver;

		this.context = context;

		this.wait = new JUnitWebDriverWait(driver, WAIT_SECONDS);

		this.drupalPagePath = getDrupalPageBase() + (pagePathSuffix != null ? "/" + pagePathSuffix: "");

		this.pageUrl = new URL(context.getBaseUri().toString() + DRUPAL_PAGE_QUERY_BASE + drupalPagePath);

		// tell browser to navigate to the page
		driver.get(pageUrl.toString());

	    // This call sets the WebElement fields.
	    PageFactory.initElements(driver, this);

		logger.info("loading " + pageUrl);

	}

	/**
	 * Creates a new PortaPage at given URL location. An Exception is thrown if
	 * this URL is not matching the expected URL for the specific page type.
	 *
	 * @param driver
	 * @param context
	 * @param url
	 * @throws Exception
	 */
	public PortalPage(WebDriver driver, DataPortalContext context, URL url) throws Exception {

		this.driver = driver;

		this.context = context;

		this.wait = new JUnitWebDriverWait(driver, 25);

		this.pageUrl = new URL(context.getBaseUri().toString());

		// tell browser to navigate to the given URL
		driver.get(url.toString());

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
	 * @param driver
	 * @param context
	 * @throws Exception
	 */
	public PortalPage(WebDriver driver, DataPortalContext context) throws Exception {

		this.driver = driver;

		this.context = context;

		this.wait = new JUnitWebDriverWait(driver, 25);

		// preliminary set the pageUrl to the base path of this page, this is used in the next setp to check if the
		// driver.getCurrentUrl() is a sub path of the base path
		this.pageUrl = new URL(context.getBaseUri().toString());

		if(!isOnPage()){
			throw new Exception("Not on the expected portal page ( current: " + driver.getCurrentUrl() + ", expected: " +  pageUrl + " )");
		}

		// now set the real URL
		this.pageUrl = new URL(driver.getCurrentUrl());

		logger.info("loading " + pageUrl);

	    // This call sets the WebElement fields.
	    PageFactory.initElements(driver, this);

	}

	/**
	 * @return
	 */
	protected boolean isOnPage() {
		return driver.getCurrentUrl().startsWith(pageUrl.toString());
	}

	public void get() {
		if(!driver.getCurrentUrl().equals(pageUrl.toString())){
			driver.get(pageUrl.toString());
			PageFactory.initElements(driver, this);
		}
	}

	public String getDrupalPagePath() {
		return drupalPagePath;
	}

	/**
	 * returns the string from the <code>title</code> tag.
	 * @return
	 */
	public String getTitle() {
		return title.getText();
	}

	/**
	 * returns the warning messages from the Drupal message box
	 * @return
	 */
	public String getWarnings() {
		return null; //TODO unimplemented
	}

	/**
	 * returns the error messages from the Drupal message box
	 * @return
	 */
	public String getErrors() {
		return null; //TODO unimplemented
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

	public void hover(WebElement element) {
		Actions actions = new Actions(driver);
		actions.moveToElement(element, 1, 1).perform();
		logger.debug("hovering");
	}


	/**
	 * Returns the current URL string from the {@link WebDriver}
	 * @return
	 */
	public URL getPageURL() {
		return pageUrl;
	}


	/**
	 * return the <code>scheme://domain:port</code> part of the initial url of this page.
	 * @return
	 */
	public String getInitialUrlBase() {
		return pageUrl.getProtocol() + "://" + pageUrl.getHost() + pageUrl.getPort();
	}

	public boolean equals(Object obj) {
		if (PortalPage.class.isAssignableFrom(obj.getClass())) {
			PortalPage page = (PortalPage) obj;
			return this.getPageURL().toString().equals(page.getPageURL().toString());

		} else {
			return false;
		}
	}


}
