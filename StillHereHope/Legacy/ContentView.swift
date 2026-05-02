import SwiftUI
import MapKit

enum ListingSource: String, CaseIterable, Identifiable, Codable {
    case housingConnect = "Housing Connect"
    case streetEasy = "StreetEasy"
    case zillow = "Zillow"

    var id: String { rawValue }

    var systemImage: String {
        switch self {
        case .housingConnect: "building.columns.fill"
        case .streetEasy: "building.2.crop.circle"
        case .zillow: "map.fill"
        }
    }

    var coverage: String {
        switch self {
        case .housingConnect: "Affordable lotteries across the five boroughs"
        case .streetEasy: "NYC rental search across the five boroughs"
        case .zillow: "Broader rental search for NYC and anywhere in New York State"
        }
    }

    var detail: String {
        switch self {
        case .housingConnect:
            "Use the official NYC Housing Connect lottery search for affordable opportunities."
        case .streetEasy:
            "Browse active NYC listings on StreetEasy when you want borough-by-borough market listings."
        case .zillow:
            "Use Zillow when you want broader New York State rental coverage beyond the five boroughs."
        }
    }

    var destinationURL: URL {
        switch self {
        case .housingConnect:
            URL(string: "https://housingconnect.nyc.gov/PublicWeb/search-lotteries")!
        case .streetEasy:
            URL(string: "https://streeteasy.com/for-rent/nyc")!
        case .zillow:
            URL(string: "https://www.zillow.com/ny/rentals/")!
        }
    }
}

struct ContentView: View {
    @StateObject private var store = HousingNavStore()
    @AppStorage("selectedHousingNavRole") private var selectedRoleRaw = ""
    @AppStorage("hasCompletedHousingNavOnboarding") private var hasCompletedOnboarding = false

    private var selectedRole: AppRole? {
        AppRole(rawValue: selectedRoleRaw)
    }

    var body: some View {
        NavigationStack {
            if !hasCompletedOnboarding {
                OnboardingView {
                    hasCompletedOnboarding = true
                }
            } else {
                if let selectedRole {
                    HousingNavTabRoot(
                        role: selectedRole,
                        onSwitchRole: {
                            selectedRoleRaw = selectedRole == .applicant ? AppRole.landlord.rawValue : AppRole.applicant.rawValue
                        },
                        onChooseRole: {
                            selectedRoleRaw = ""
                        }
                    )
                } else {
                    RoleSelectionView { role in
                        selectedRoleRaw = role.rawValue
                    }
                }
            }
        }
        .environmentObject(store)
        .tint(AppTheme.ink)
    }
}

struct HousingNavTabRoot: View {
    let role: AppRole
    let onSwitchRole: () -> Void
    let onChooseRole: () -> Void

    var body: some View {
        TabView {
            NavigationStack {
                switch role {
                case .applicant:
                    ApplicantDashboardView(onSwitchRole: onSwitchRole, onChooseRole: onChooseRole)
                case .landlord:
                    LandlordDashboardView(onSwitchRole: onSwitchRole, onChooseRole: onChooseRole)
                }
            }
            .tabItem {
                Label("Home", systemImage: "house.fill")
            }

            NavigationStack {
                ListingsView()
            }
            .tabItem {
                Label("Listings", systemImage: "building.2.fill")
            }

            NavigationStack {
                CompareListingsView()
            }
            .tabItem {
                Label("Compare", systemImage: "rectangle.split.3x1.fill")
            }

            NavigationStack {
                SupportView()
            }
            .tabItem {
                Label("Support", systemImage: "questionmark.circle.fill")
            }
        }
    }
}

enum AppRole: String, Codable, CaseIterable, Identifiable {
    case applicant = "Applicant / Tenant"
    case landlord = "Landlord"

    var id: String { rawValue }

    var shortTitle: String {
        switch self {
        case .applicant: "Applicant"
        case .landlord: "Landlord"
        }
    }

    var symbol: String {
        switch self {
        case .applicant: "person.text.rectangle"
        case .landlord: "building.2.crop.circle"
        }
    }
}

enum YesNoAnswer: String, Codable, CaseIterable, Identifiable {
    case notAnswered = "Not answered"
    case yes = "Yes"
    case no = "No"

    var id: String { rawValue }
}

struct HousingListing: Identifiable, Equatable, Codable {
    let id: String
    let title: String
    let location: String
    let monthlyRent: Int?
    let bedrooms: Int
    let bedroomSummary: String
    let summary: String
    let source: ListingSource
    let borough: String?
    let statusText: String?
    let deadlineText: String?
    let deadlineDate: Date?
    let urgencyText: String?
    let developmentType: String?
    let unitCount: Int?
    let unitMixDetails: [String]
    let amiSummary: String?
    let listingURL: URL?
    let coordinate: CLLocationCoordinate2D

    enum CodingKeys: String, CodingKey {
        case id
        case title
        case location
        case monthlyRent
        case bedrooms
        case bedroomSummary
        case summary
        case source
        case borough
        case statusText
        case deadlineText
        case deadlineDate
        case urgencyText
        case developmentType
        case unitCount
        case unitMixDetails
        case amiSummary
        case listingURL
        case latitude
        case longitude
    }

    init(
        id: String,
        title: String,
        location: String,
        monthlyRent: Int?,
        bedrooms: Int,
        bedroomSummary: String,
        summary: String,
        source: ListingSource,
        borough: String?,
        statusText: String?,
        deadlineText: String?,
        deadlineDate: Date?,
        urgencyText: String?,
        developmentType: String?,
        unitCount: Int?,
        unitMixDetails: [String],
        amiSummary: String?,
        listingURL: URL?,
        coordinate: CLLocationCoordinate2D
    ) {
        self.id = id
        self.title = title
        self.location = location
        self.monthlyRent = monthlyRent
        self.bedrooms = bedrooms
        self.bedroomSummary = bedroomSummary
        self.summary = summary
        self.source = source
        self.borough = borough
        self.statusText = statusText
        self.deadlineText = deadlineText
        self.deadlineDate = deadlineDate
        self.urgencyText = urgencyText
        self.developmentType = developmentType
        self.unitCount = unitCount
        self.unitMixDetails = unitMixDetails
        self.amiSummary = amiSummary
        self.listingURL = listingURL
        self.coordinate = coordinate
    }

    init(from decoder: Decoder) throws {
        let container = try decoder.container(keyedBy: CodingKeys.self)
        id = try container.decode(String.self, forKey: .id)
        title = try container.decode(String.self, forKey: .title)
        location = try container.decode(String.self, forKey: .location)
        monthlyRent = try container.decodeIfPresent(Int.self, forKey: .monthlyRent)
        bedrooms = try container.decode(Int.self, forKey: .bedrooms)
        bedroomSummary = try container.decode(String.self, forKey: .bedroomSummary)
        summary = try container.decode(String.self, forKey: .summary)
        source = try container.decode(ListingSource.self, forKey: .source)
        borough = try container.decodeIfPresent(String.self, forKey: .borough)
        statusText = try container.decodeIfPresent(String.self, forKey: .statusText)
        deadlineText = try container.decodeIfPresent(String.self, forKey: .deadlineText)
        deadlineDate = try container.decodeIfPresent(Date.self, forKey: .deadlineDate)
        urgencyText = try container.decodeIfPresent(String.self, forKey: .urgencyText)
        developmentType = try container.decodeIfPresent(String.self, forKey: .developmentType)
        unitCount = try container.decodeIfPresent(Int.self, forKey: .unitCount)
        unitMixDetails = try container.decodeIfPresent([String].self, forKey: .unitMixDetails) ?? []
        amiSummary = try container.decodeIfPresent(String.self, forKey: .amiSummary)
        listingURL = try container.decodeIfPresent(URL.self, forKey: .listingURL)
        let latitude = try container.decode(CLLocationDegrees.self, forKey: .latitude)
        let longitude = try container.decode(CLLocationDegrees.self, forKey: .longitude)
        coordinate = CLLocationCoordinate2D(latitude: latitude, longitude: longitude)
    }

    func encode(to encoder: Encoder) throws {
        var container = encoder.container(keyedBy: CodingKeys.self)
        try container.encode(id, forKey: .id)
        try container.encode(title, forKey: .title)
        try container.encode(location, forKey: .location)
        try container.encodeIfPresent(monthlyRent, forKey: .monthlyRent)
        try container.encode(bedrooms, forKey: .bedrooms)
        try container.encode(bedroomSummary, forKey: .bedroomSummary)
        try container.encode(summary, forKey: .summary)
        try container.encode(source, forKey: .source)
        try container.encodeIfPresent(borough, forKey: .borough)
        try container.encodeIfPresent(statusText, forKey: .statusText)
        try container.encodeIfPresent(deadlineText, forKey: .deadlineText)
        try container.encodeIfPresent(deadlineDate, forKey: .deadlineDate)
        try container.encodeIfPresent(urgencyText, forKey: .urgencyText)
        try container.encodeIfPresent(developmentType, forKey: .developmentType)
        try container.encodeIfPresent(unitCount, forKey: .unitCount)
        try container.encode(unitMixDetails, forKey: .unitMixDetails)
        try container.encodeIfPresent(amiSummary, forKey: .amiSummary)
        try container.encodeIfPresent(listingURL, forKey: .listingURL)
        try container.encode(coordinate.latitude, forKey: .latitude)
        try container.encode(coordinate.longitude, forKey: .longitude)
    }

    static func == (lhs: HousingListing, rhs: HousingListing) -> Bool {
        lhs.id == rhs.id
    }
}

enum ListingCatalog {
    static let all: [HousingListing] = [
        HousingListing(
            id: "bronx-garden-2br",
            title: "Bronx Garden 2BR",
            location: "Bronx, NY",
            monthlyRent: 2450,
            bedrooms: 2,
            bedroomSummary: "2 bedrooms",
            summary: "Voucher-friendly planning sample near transit with elevator building notes.",
            source: .housingConnect,
            borough: "Bronx",
            statusText: "Planning sample",
            deadlineText: nil,
            deadlineDate: nil,
            urgencyText: nil,
            developmentType: "Rental",
            unitCount: 1,
            unitMixDetails: ["2BR sample"],
            amiSummary: nil,
            listingURL: nil,
            coordinate: CLLocationCoordinate2D(latitude: 40.8448, longitude: -73.8648)
        ),
        HousingListing(
            id: "queens-sunny-1br",
            title: "Queens Sunny 1BR",
            location: "Queens, NY",
            monthlyRent: 2180,
            bedrooms: 1,
            bedroomSummary: "1 bedroom",
            summary: "Good fit for saved checklist prep, document review, and commute comparison.",
            source: .streetEasy,
            borough: "Queens",
            statusText: "Planning sample",
            deadlineText: nil,
            deadlineDate: nil,
            urgencyText: nil,
            developmentType: "Rental",
            unitCount: 1,
            unitMixDetails: ["1BR sample"],
            amiSummary: nil,
            listingURL: URL(string: "https://streeteasy.com/for-rent/nyc")!,
            coordinate: CLLocationCoordinate2D(latitude: 40.7282, longitude: -73.7949)
        ),
        HousingListing(
            id: "brooklyn-accessible-studio",
            title: "Brooklyn Accessible Studio",
            location: "Brooklyn, NY",
            monthlyRent: 1985,
            bedrooms: 0,
            bedroomSummary: "Studio",
            summary: "Studio sample with accessibility notes and inspection-ready reminders.",
            source: .housingConnect,
            borough: "Brooklyn",
            statusText: "Planning sample",
            deadlineText: nil,
            deadlineDate: nil,
            urgencyText: nil,
            developmentType: "Rental",
            unitCount: 1,
            unitMixDetails: ["Studio sample"],
            amiSummary: nil,
            listingURL: nil,
            coordinate: CLLocationCoordinate2D(latitude: 40.6782, longitude: -73.9442)
        ),
        HousingListing(
            id: "manhattan-family-3br",
            title: "Manhattan Family 3BR",
            location: "Manhattan, NY",
            monthlyRent: 3750,
            bedrooms: 3,
            bedroomSummary: "3 bedrooms",
            summary: "Larger household sample for comparing rent, bedroom size, and paperwork needs.",
            source: .zillow,
            borough: "Manhattan",
            statusText: "Planning sample",
            deadlineText: nil,
            deadlineDate: nil,
            urgencyText: nil,
            developmentType: "Rental",
            unitCount: 1,
            unitMixDetails: ["3BR sample"],
            amiSummary: nil,
            listingURL: URL(string: "https://www.zillow.com/ny/rentals/")!,
            coordinate: CLLocationCoordinate2D(latitude: 40.7831, longitude: -73.9712)
        )
    ]
}

private struct HousingConnectLotteryRecord: Decodable {
    let lotteryID: String
    let lotteryName: String
    let lotteryStatus: String
    let developmentType: String?
    let lotteryStartDate: String?
    let lotteryEndDate: String?
    let unitCount: String?
    let studioUnits: String?
    let oneBedUnits: String?
    let twoBedUnits: String?
    let threeBedUnits: String?
    let fourBedUnits: String?
    let amiVeryLow: String?
    let amiLow: String?
    let amiModerate: String?
    let amiMiddle: String?
    let borough: String
    let postcode: String?
    let latitude: String?
    let longitude: String?

    enum CodingKeys: String, CodingKey {
        case lotteryID = "lottery_id"
        case lotteryName = "lottery_name"
        case lotteryStatus = "lottery_status"
        case developmentType = "development_type"
        case lotteryStartDate = "lottery_start_date"
        case lotteryEndDate = "lottery_end_date"
        case unitCount = "unit_count"
        case studioUnits = "unit_distribution_studio"
        case oneBedUnits = "unit_distribution_1bed"
        case twoBedUnits = "unit_distribution_2bed"
        case threeBedUnits = "unit_distribution_3bed"
        case fourBedUnits = "unit_distribution_4bed"
        case amiVeryLow = "applied_income_ami_very_low"
        case amiLow = "applied_income_ami_low"
        case amiModerate = "applied_income_ami_moderate"
        case amiMiddle = "applied_income_ami_middle"
        case borough
        case postcode
        case latitude
        case longitude
    }

    func toListing() -> HousingListing? {
        guard
            let latitude,
            let longitude,
            latitude != "Multiple",
            longitude != "Multiple",
            let lat = Double(latitude),
            let lon = Double(longitude)
        else {
            return nil
        }

        let bedroomMix = availableBedroomMix
        let boroughName = Self.boroughName(from: borough)
        let locationName = "\(boroughName), NY"
        let parsedUnitCount = Int(unitCount ?? "")
        let unitLabel = parsedUnitCount.map { "\($0) units" } ?? "Units listed"
        let summaryParts = [
            developmentType.map { "\($0) lottery" },
            unitLabel,
            bedroomMix == "Various layouts" ? nil : bedroomMix
        ].compactMap { $0 }
        let deadlineDate = Self.date(from: lotteryEndDate)

        return HousingListing(
            id: "housing-connect-\(lotteryID)",
            title: lotteryName,
            location: locationName,
            monthlyRent: nil,
            bedrooms: maxBedrooms,
            bedroomSummary: bedroomMix,
            summary: summaryParts.joined(separator: " • "),
            source: .housingConnect,
            borough: boroughName,
            statusText: lotteryStatus,
            deadlineText: Self.shortDate(lotteryEndDate),
            deadlineDate: deadlineDate,
            urgencyText: Self.urgencyText(for: deadlineDate),
            developmentType: developmentType,
            unitCount: parsedUnitCount,
            unitMixDetails: unitMixDetails,
            amiSummary: amiSummary,
            listingURL: ListingSource.housingConnect.destinationURL,
            coordinate: CLLocationCoordinate2D(latitude: lat, longitude: lon)
        )
    }

    private var availableBedroomMix: String {
        let mixes = unitMixDetails.map { $0.components(separatedBy: " ").first ?? $0 }
        return mixes.isEmpty ? "Various layouts" : mixes.joined(separator: ", ")
    }

    private var unitMixDetails: [String] {
        [
            (studioUnits, "Studio"),
            (oneBedUnits, "1BR"),
            (twoBedUnits, "2BR"),
            (threeBedUnits, "3BR"),
            (fourBedUnits, "4BR")
        ].compactMap { count, label -> String? in
            guard let count, Int(count) ?? 0 > 0 else { return nil }
            return "\(label) x \(count)"
        }
    }

    private var amiSummary: String? {
        let details = [
            (amiVeryLow, "Very low AMI"),
            (amiLow, "Low AMI"),
            (amiModerate, "Moderate AMI"),
            (amiMiddle, "Middle AMI")
        ].compactMap { count, label -> String? in
            guard let count, Int(count) ?? 0 > 0 else { return nil }
            return "\(label): \(count)"
        }
        return details.isEmpty ? nil : details.joined(separator: " • ")
    }

    private var maxBedrooms: Int {
        if Int(fourBedUnits ?? "") ?? 0 > 0 { return 4 }
        if Int(threeBedUnits ?? "") ?? 0 > 0 { return 3 }
        if Int(twoBedUnits ?? "") ?? 0 > 0 { return 2 }
        if Int(oneBedUnits ?? "") ?? 0 > 0 { return 1 }
        return 0
    }

    private static func boroughName(from value: String) -> String {
        switch value {
        case "BK": "Brooklyn"
        case "BX": "Bronx"
        case "MN": "Manhattan"
        case "QN": "Queens"
        case "SI": "Staten Island"
        default: value
        }
    }

    private static func shortDate(_ value: String?) -> String? {
        guard let date = date(from: value) else { return nil }
        return date.formatted(date: .abbreviated, time: .omitted)
    }

    private static func date(from value: String?) -> Date? {
        guard let value else { return nil }
        let formatter = ISO8601DateFormatter()
        return formatter.date(from: value)
    }

    private static func urgencyText(for deadlineDate: Date?) -> String? {
        guard let deadlineDate else { return nil }
        let days = Calendar.current.dateComponents([.day], from: Calendar.current.startOfDay(for: .now), to: Calendar.current.startOfDay(for: deadlineDate)).day ?? 0
        switch days {
        case ..<0: return "Closed recently"
        case 0: return "Closes today"
        case 1...7: return "Closing soon"
        case 8...21: return "Open now"
        default: return nil
        }
    }
}

enum HousingConnectFeedState: Equatable {
    case idle
    case loading
    case loaded(Date)
    case failed(String)
}

enum ListingSortMode: String, CaseIterable, Identifiable {
    case closingSoonest = "Closing soonest"
    case borough = "Borough"
    case alphabetical = "A to Z"

    var id: String { rawValue }
}

enum VoucherProgram: String, Codable, CaseIterable, Identifiable {
    case section8 = "Section 8"
    case cityFHEPS = "CityFHEPS"
    case fheps = "FHEPS"

    var id: String { rawValue }

    var note: String {
        switch self {
        case .section8:
            "Uses embedded FY 2026 NYCHA Section 8 payment standards, with supported ZIP exception standards when available."
        case .cityFHEPS:
            "Uses DSS 2026 CityFHEPS standards and utility allowances for current package timing."
        case .fheps:
            "Uses the current public FHEPS/CityFHEPS 2025 utility and payment schedule until updated agency guidance is available."
        }
    }
}

enum BedroomSize: String, Codable, CaseIterable, Identifiable {
    case sro = "SRO"
    case studio = "Studio"
    case one = "1 bedroom"
    case two = "2 bedrooms"
    case three = "3 bedrooms"
    case four = "4 bedrooms"
    case five = "5 bedrooms"
    case six = "6 bedrooms"
    case seven = "7 bedrooms"
    case eight = "8 bedrooms"

    var id: String { rawValue }

    var key: String {
        switch self {
        case .sro: "sro"
        case .studio: "studio"
        case .one: "1"
        case .two: "2"
        case .three: "3"
        case .four: "4"
        case .five: "5"
        case .six: "6"
        case .seven: "7"
        case .eight: "8"
        }
    }
}

enum UtilityShortcut: String, Codable, CaseIterable, Identifiable {
    case landlordPaysAll = "Landlord pays all utilities"
    case tenantPaysAllGas = "Tenant pays all: gas profile"
    case tenantPaysAllElectric = "Tenant pays all: electric profile"
    case custom = "Select utilities manually"

    var id: String { rawValue }
}

enum CookingUtility: String, Codable, CaseIterable, Identifiable {
    case none = "Landlord pays cooking"
    case gas = "Tenant pays cooking gas"
    case electric = "Tenant pays electric cooking"
    case manual = "Other cooking setup"

    var id: String { rawValue }
}

enum OtherElectricUtility: String, Codable, CaseIterable, Identifiable {
    case no = "Landlord pays electric"
    case yes = "Tenant pays electric"

    var id: String { rawValue }
}

enum Section8HeatHotWater: String, Codable, CaseIterable, Identifiable {
    case none = "Landlord pays heat and hot water"
    case gasHotWater = "Tenant pays gas hot water only"
    case gasHeat = "Tenant pays gas heat only"
    case gas = "Tenant pays gas heat and hot water"
    case oilHotWater = "Tenant pays oil hot water only"
    case oilHeat = "Tenant pays oil heat only"
    case oil = "Tenant pays oil heat and hot water"
    case electricHotWater = "Tenant pays electric hot water only"
    case electricHeat = "Tenant pays electric heat only"
    case electric = "Tenant pays electric heat and hot water"
    case heatPump = "Tenant pays heat pump utility"
    case manual = "Other heat or hot water setup"

    var id: String { rawValue }
}

enum HeatUtility: String, Codable, CaseIterable, Identifiable {
    case none = "Landlord pays"
    case gas = "Tenant pays gas"
    case oil = "Tenant pays oil"
    case electric = "Tenant pays electric"
    case manual = "Other setup"

    var id: String { rawValue }
}

struct UtilitySchedule {
    var gas: Double = 0
    var electric: Double = 0
    var gasElectric: Double = 0
    var cookingGas: Double = 0
    var electricWithRange: Double = 0
    var oilHotWater: Double = 0
    var oilHeat: Double = 0
    var oilTotal: Double = 0
    var gasHotWater: Double = 0
    var gasHeat: Double = 0
    var gasTotal: Double = 0
    var electricHotWater: Double = 0
    var electricHeat: Double = 0
    var electricTotal: Double = 0
    var heatPump: Double = 0
}

enum HousingCalculatorEngine {
    static let nycha2026Standards: [String: Double] = [
        "sro": 1953, "studio": 2604, "1": 2734, "2": 2997, "3": 3753,
        "4": 4077, "5": 4689, "6": 5301, "7": 5913, "8": 6524
    ]

    static let cityFHEPS2026Standards: [String: Double] = [
        "sro": 1953, "studio": 2604, "1": 2734, "2": 2997, "3": 3753,
        "4": 4077, "5": 4689, "6": 5301, "7": 5913, "8": 6524
    ]

    static let fheps2025Standards: [String: Double] = [
        "sro": 1985, "studio": 2646, "1": 2762, "2": 3058, "3": 3811,
        "4": 4111, "5": 4728, "6": 5345, "7": 5962, "8": 6579
    ]

    static let nychaExceptionStandards: [String: [String: Double]] = [
        "10001": ["sro": 2935, "studio": 3914, "1": 4109, "2": 4501, "3": 5634, "4": 6128, "5": 7048, "6": 7967, "7": 8886, "8": 9805],
        "10003": ["sro": 2935, "studio": 3914, "1": 4109, "2": 4501, "3": 5634, "4": 6128, "5": 7048, "6": 7967, "7": 8886, "8": 9805],
        "10004": ["sro": 2410, "studio": 3213, "1": 3368, "2": 3697, "3": 4635, "4": 5026, "5": 5780, "6": 6534, "7": 7288, "8": 8042],
        "10005": ["sro": 2511, "studio": 3347, "1": 3502, "2": 3872, "3": 4820, "4": 5201, "5": 5982, "6": 6761, "7": 7542, "8": 8322],
        "10011": ["sro": 2935, "studio": 3914, "1": 4109, "2": 4501, "3": 5634, "4": 6128, "5": 7048, "6": 7967, "7": 8886, "8": 9805],
        "10012": ["sro": 2935, "studio": 3914, "1": 4109, "2": 4501, "3": 5634, "4": 6128, "5": 7048, "6": 7967, "7": 8886, "8": 9805],
        "10013": ["sro": 2935, "studio": 3914, "1": 4109, "2": 4501, "3": 5634, "4": 6128, "5": 7048, "6": 7967, "7": 8886, "8": 9805],
        "10014": ["sro": 2935, "studio": 3914, "1": 4109, "2": 4501, "3": 5634, "4": 6128, "5": 7048, "6": 7967, "7": 8886, "8": 9805],
        "10016": ["sro": 2935, "studio": 3914, "1": 4109, "2": 4501, "3": 5634, "4": 6128, "5": 7048, "6": 7967, "7": 8886, "8": 9805],
        "10017": ["sro": 2935, "studio": 3914, "1": 4109, "2": 4501, "3": 5634, "4": 6128, "5": 7048, "6": 7967, "7": 8886, "8": 9805],
        "10018": ["sro": 2851, "studio": 3800, "1": 3996, "2": 4367, "3": 5469, "4": 5953, "5": 6846, "6": 7739, "7": 8632, "8": 9525],
        "10019": ["sro": 2742, "studio": 3656, "1": 3831, "2": 4202, "3": 5263, "4": 5716, "5": 6574, "6": 7431, "7": 8289, "8": 9146],
        "10021": ["sro": 2935, "studio": 3914, "1": 4109, "2": 4501, "3": 5634, "4": 6128, "5": 7048, "6": 7967, "7": 8886, "8": 9805],
        "10022": ["sro": 2935, "studio": 3914, "1": 4109, "2": 4501, "3": 5634, "4": 6128, "5": 7048, "6": 7967, "7": 8886, "8": 9805],
        "10023": ["sro": 2935, "studio": 3914, "1": 4109, "2": 4501, "3": 5634, "4": 6128, "5": 7048, "6": 7967, "7": 8886, "8": 9805],
        "10024": ["sro": 2935, "studio": 3914, "1": 4109, "2": 4501, "3": 5634, "4": 6128, "5": 7048, "6": 7967, "7": 8886, "8": 9805],
        "10028": ["sro": 2935, "studio": 3914, "1": 4109, "2": 4501, "3": 5634, "4": 6128, "5": 7048, "6": 7967, "7": 8886, "8": 9805],
        "10036": ["sro": 2711, "studio": 3615, "1": 3800, "2": 4161, "3": 5211, "4": 5665, "5": 6514, "6": 7364, "7": 8214, "8": 9064],
        "10038": ["sro": 2935, "studio": 3914, "1": 4109, "2": 4501, "3": 5634, "4": 6128, "5": 7048, "6": 7967, "7": 8886, "8": 9805],
        "10044": ["sro": 2935, "studio": 3914, "1": 4109, "2": 4501, "3": 5634, "4": 6128, "5": 7048, "6": 7967, "7": 8886, "8": 9805],
        "10065": ["sro": 2935, "studio": 3914, "1": 4109, "2": 4501, "3": 5634, "4": 6128, "5": 7048, "6": 7967, "7": 8886, "8": 9805],
        "10069": ["sro": 2935, "studio": 3914, "1": 4109, "2": 4501, "3": 5634, "4": 6128, "5": 7048, "6": 7967, "7": 8886, "8": 9805],
        "10075": ["sro": 2487, "studio": 3316, "1": 3481, "2": 3821, "3": 4789, "4": 5201, "5": 5982, "6": 6761, "7": 7542, "8": 8322],
        "10301": ["sro": 2039, "studio": 2719, "1": 2853, "2": 3131, "3": 3924, "4": 4264, "5": 4903, "6": 5543, "7": 6183, "8": 6822],
        "10309": ["sro": 2209, "studio": 2945, "1": 3090, "2": 3388, "3": 4243, "4": 4614, "5": 5306, "6": 5998, "7": 6690, "8": 7383],
        "10461": ["sro": 2023, "studio": 2698, "1": 2832, "2": 3110, "3": 3893, "4": 4233, "5": 4868, "6": 5503, "7": 6138, "8": 6773],
        "10464": ["sro": 1970, "studio": 2626, "1": 2760, "2": 3028, "3": 3790, "4": 4120, "5": 4738, "6": 5356, "7": 5974, "8": 6592],
        "10465": ["sro": 2039, "studio": 2719, "1": 2853, "2": 3131, "3": 3924, "4": 4264, "5": 4903, "6": 5543, "7": 6183, "8": 6822],
        "11209": ["sro": 2178, "studio": 2904, "1": 3048, "2": 3337, "3": 4181, "4": 4542, "5": 5224, "6": 5904, "7": 6586, "8": 7267],
        "11211": ["sro": 2425, "studio": 3234, "1": 3388, "2": 3718, "3": 4655, "4": 5057, "5": 5816, "6": 6574, "7": 7333, "8": 8091],
        "11215": ["sro": 2935, "studio": 3914, "1": 4109, "2": 4501, "3": 5634, "4": 6128, "5": 7048, "6": 7967, "7": 8886, "8": 9805],
        "11216": ["sro": 2464, "studio": 3285, "1": 3450, "2": 3780, "3": 4738, "4": 5139, "5": 5911, "6": 6681, "7": 7453, "8": 8223],
        "11217": ["sro": 2935, "studio": 3914, "1": 4109, "2": 4501, "3": 5634, "4": 6128, "5": 7048, "6": 7967, "7": 8886, "8": 9805],
        "11218": ["sro": 2140, "studio": 2853, "1": 2997, "2": 3285, "3": 4109, "4": 4470, "5": 5140, "6": 5811, "7": 6481, "8": 7152],
        "11221": ["sro": 2124, "studio": 2832, "1": 2966, "2": 3254, "3": 4078, "4": 4429, "5": 5093, "6": 5757, "7": 6422, "8": 7086],
        "11222": ["sro": 2672, "studio": 3563, "1": 3738, "2": 4099, "3": 5129, "4": 5582, "5": 6419, "6": 7257, "7": 8094, "8": 8932],
        "11230": ["sro": 2047, "studio": 2729, "1": 2863, "2": 3141, "3": 3934, "4": 4274, "5": 4916, "6": 5556, "7": 6198, "8": 6839],
        "11231": ["sro": 2935, "studio": 3914, "1": 4109, "2": 4501, "3": 5634, "4": 6128, "5": 7048, "6": 7967, "7": 8886, "8": 9805],
        "11237": ["sro": 2023, "studio": 2698, "1": 2832, "2": 3110, "3": 3893, "4": 4233, "5": 4868, "6": 5503, "7": 6138, "8": 6773],
        "11238": ["sro": 2789, "studio": 3718, "1": 3903, "2": 4284, "3": 5366, "4": 5829, "5": 6704, "6": 7578, "7": 8453, "8": 9327],
        "11354": ["sro": 2023, "studio": 2698, "1": 2832, "2": 3110, "3": 3893, "4": 4233, "5": 4868, "6": 5503, "7": 6138, "8": 6773],
        "11355": ["sro": 2023, "studio": 2698, "1": 2832, "2": 3100, "3": 3883, "4": 4223, "5": 4856, "6": 5489, "7": 6123, "8": 6756],
        "11356": ["sro": 2178, "studio": 2904, "1": 3048, "2": 3347, "3": 4192, "4": 4552, "5": 5235, "6": 5918, "7": 6601, "8": 7284],
        "11357": ["sro": 2279, "studio": 3038, "1": 3182, "2": 3491, "3": 4377, "4": 4748, "5": 5461, "6": 6172, "7": 6885, "8": 7597],
        "11358": ["sro": 2171, "studio": 2894, "1": 3038, "2": 3326, "3": 4161, "4": 4521, "5": 5200, "6": 5878, "7": 6556, "8": 7234],
        "11360": ["sro": 2464, "studio": 3285, "1": 3450, "2": 3780, "3": 4738, "4": 5139, "5": 5911, "6": 6681, "7": 7453, "8": 8223],
        "11361": ["sro": 2418, "studio": 3223, "1": 3378, "2": 3708, "3": 4645, "4": 5047, "5": 5804, "6": 6561, "7": 7318, "8": 8075],
        "11362": ["sro": 2626, "studio": 3502, "1": 3677, "2": 4027, "3": 5047, "4": 5479, "5": 6301, "6": 7123, "7": 7945, "8": 8767]
    ]

    static let section8Utilities: [String: UtilitySchedule] = [
        "studio": UtilitySchedule(gas: 29, electric: 96, gasElectric: 125, electricWithRange: 111, oilHotWater: 28, oilHeat: 91, oilTotal: 119, gasHotWater: 22, gasHeat: 73, gasTotal: 95, electricHotWater: 37, electricHeat: 52, electricTotal: 89, heatPump: 31),
        "1": UtilitySchedule(gas: 33, electric: 108, gasElectric: 141, electricWithRange: 126, oilHotWater: 32, oilHeat: 107, oilTotal: 139, gasHotWater: 26, gasHeat: 86, gasTotal: 112, electricHotWater: 44, electricHeat: 62, electricTotal: 106, heatPump: 37),
        "2": UtilitySchedule(gas: 39, electric: 142, gasElectric: 181, electricWithRange: 167, oilHotWater: 47, oilHeat: 122, oilTotal: 169, gasHotWater: 38, gasHeat: 100, gasTotal: 138, electricHotWater: 56, electricHeat: 82, electricTotal: 138, heatPump: 40),
        "3": UtilitySchedule(gas: 45, electric: 176, gasElectric: 221, electricWithRange: 209, oilHotWater: 61, oilHeat: 138, oilTotal: 199, gasHotWater: 49, gasHeat: 110, gasTotal: 159, electricHotWater: 68, electricHeat: 103, electricTotal: 171, heatPump: 47),
        "4": UtilitySchedule(gas: 51, electric: 210, gasElectric: 261, electricWithRange: 251, oilHotWater: 76, oilHeat: 153, oilTotal: 229, gasHotWater: 61, gasHeat: 122, gasTotal: 183, electricHotWater: 80, electricHeat: 123, electricTotal: 203, heatPump: 55),
        "5": UtilitySchedule(gas: 57, electric: 244, gasElectric: 301, electricWithRange: 293, oilHotWater: 90, oilHeat: 168, oilTotal: 258, gasHotWater: 72, gasHeat: 134, gasTotal: 206, electricHotWater: 93, electricHeat: 144, electricTotal: 237, heatPump: 65)
    ]

    static let cityFHEPSUtilities: [String: UtilitySchedule] = [
        "studio": UtilitySchedule(electric: 96, gasElectric: 125, cookingGas: 29, electricWithRange: 111, oilHotWater: 28, oilHeat: 91, oilTotal: 119, gasHotWater: 22, gasHeat: 73, gasTotal: 95, electricHotWater: 37, electricHeat: 52, electricTotal: 89, heatPump: 31),
        "1": UtilitySchedule(electric: 108, gasElectric: 141, cookingGas: 33, electricWithRange: 126, oilHotWater: 32, oilHeat: 107, oilTotal: 139, gasHotWater: 26, gasHeat: 86, gasTotal: 112, electricHotWater: 44, electricHeat: 62, electricTotal: 106, heatPump: 37),
        "2": UtilitySchedule(electric: 142, gasElectric: 181, cookingGas: 39, electricWithRange: 167, oilHotWater: 47, oilHeat: 122, oilTotal: 169, gasHotWater: 38, gasHeat: 100, gasTotal: 138, electricHotWater: 56, electricHeat: 82, electricTotal: 138, heatPump: 40),
        "3": UtilitySchedule(electric: 176, gasElectric: 221, cookingGas: 45, electricWithRange: 209, oilHotWater: 61, oilHeat: 138, oilTotal: 199, gasHotWater: 49, gasHeat: 110, gasTotal: 159, electricHotWater: 68, electricHeat: 103, electricTotal: 171, heatPump: 47),
        "4": UtilitySchedule(electric: 210, gasElectric: 261, cookingGas: 51, electricWithRange: 251, oilHotWater: 76, oilHeat: 153, oilTotal: 229, gasHotWater: 61, gasHeat: 122, gasTotal: 183, electricHotWater: 80, electricHeat: 123, electricTotal: 203, heatPump: 55),
        "5": UtilitySchedule(electric: 244, gasElectric: 301, cookingGas: 57, electricWithRange: 293, oilHotWater: 90, oilHeat: 168, oilTotal: 258, gasHotWater: 72, gasHeat: 134, gasTotal: 206, electricHotWater: 93, electricHeat: 144, electricTotal: 237, heatPump: 65)
    ]

    static let fhepsUtilities: [String: UtilitySchedule] = [
        "studio": UtilitySchedule(electric: 96, gasElectric: 122, cookingGas: 26, electricWithRange: 111, oilHotWater: 29, oilHeat: 94, oilTotal: 123, gasHotWater: 20, gasHeat: 65, gasTotal: 85, electricHotWater: 38, electricHeat: 54, electricTotal: 92),
        "1": UtilitySchedule(electric: 109, gasElectric: 138, cookingGas: 29, electricWithRange: 127, oilHotWater: 34, oilHeat: 111, oilTotal: 145, gasHotWater: 23, gasHeat: 76, gasTotal: 99, electricHotWater: 45, electricHeat: 63, electricTotal: 108),
        "2": UtilitySchedule(electric: 144, gasElectric: 177, cookingGas: 33, electricWithRange: 170, oilHotWater: 49, oilHeat: 127, oilTotal: 176, gasHotWater: 33, gasHeat: 88, gasTotal: 121, electricHotWater: 57, electricHeat: 84, electricTotal: 141),
        "3": UtilitySchedule(electric: 178, gasElectric: 215, cookingGas: 37, electricWithRange: 212, oilHotWater: 64, oilHeat: 142, oilTotal: 206, gasHotWater: 44, gasHeat: 98, gasTotal: 142, electricHotWater: 70, electricHeat: 105, electricTotal: 175),
        "4": UtilitySchedule(electric: 213, gasElectric: 254, cookingGas: 41, electricWithRange: 255, oilHotWater: 79, oilHeat: 158, oilTotal: 237, gasHotWater: 54, gasHeat: 109, gasTotal: 163, electricHotWater: 82, electricHeat: 126, electricTotal: 208),
        "5": UtilitySchedule(electric: 248, gasElectric: 293, cookingGas: 45, electricWithRange: 298, oilHotWater: 93, oilHeat: 174, oilTotal: 267, gasHotWater: 64, gasHeat: 119, gasTotal: 183, electricHotWater: 95, electricHeat: 147, electricTotal: 242)
    ]

    static func paymentStandard(program: VoucherProgram, bedroom: BedroomSize, zipCode: String) -> Double {
        if program == .section8, let exception = nychaExceptionStandards[zipCode.trimmingCharacters(in: .whitespacesAndNewlines)]?[bedroom.key] {
            return exception
        }

        switch program {
        case .section8:
            return nycha2026Standards[bedroom.key] ?? 0
        case .cityFHEPS:
            return cityFHEPS2026Standards[bedroom.key] ?? 0
        case .fheps:
            return fheps2025Standards[bedroom.key] ?? 0
        }
    }

    static func section8UtilityAllowance(bedroom: BedroomSize, cooking: CookingUtility, otherElectric: OtherElectricUtility, heatHotWater: Section8HeatHotWater) -> Double {
        let schedule = section8Utilities[bedroom.key] ?? section8Utilities["5"] ?? UtilitySchedule()
        var total = 0.0

        if cooking == .gas && otherElectric == .yes {
            total += schedule.gasElectric
        } else if cooking == .electric && otherElectric == .yes {
            total += schedule.electricWithRange
        } else {
            if cooking == .gas { total += schedule.gas }
            if otherElectric == .yes || cooking == .electric { total += schedule.electric }
        }

        switch heatHotWater {
        case .gasHotWater: total += schedule.gasHotWater
        case .gasHeat: total += schedule.gasHeat
        case .gas: total += schedule.gasTotal
        case .oilHotWater: total += schedule.oilHotWater
        case .oilHeat: total += schedule.oilHeat
        case .oil: total += schedule.oilTotal
        case .electricHotWater: total += schedule.electricHotWater
        case .electricHeat: total += schedule.electricHeat
        case .electric: total += schedule.electricTotal
        case .heatPump: total += schedule.heatPump
        case .none, .manual: break
        }

        return total
    }

    static func nonSection8UtilityAllowance(program: VoucherProgram, bedroom: BedroomSize, cooking: CookingUtility, otherElectric: OtherElectricUtility, heating: HeatUtility, hotWater: HeatUtility) -> Double {
        let schedules = program == .fheps ? fhepsUtilities : cityFHEPSUtilities
        let schedule = schedules[bedroom.key] ?? schedules["5"] ?? UtilitySchedule()
        var total = 0.0

        if otherElectric == .yes && cooking == .gas {
            total += schedule.gasElectric
        } else if otherElectric == .yes && cooking == .electric {
            total += schedule.electricWithRange
        } else {
            if cooking == .gas { total += schedule.cookingGas }
            if otherElectric == .yes || cooking == .electric { total += schedule.electric }
        }

        if heating == .oil && hotWater == .oil {
            total += schedule.oilTotal
        } else {
            if heating == .oil { total += schedule.oilHeat }
            if hotWater == .oil { total += schedule.oilHotWater }
        }

        if heating == .gas && hotWater == .gas {
            total += schedule.gasTotal
        } else {
            if heating == .gas { total += schedule.gasHeat }
            if hotWater == .gas { total += schedule.gasHotWater }
        }

        if heating == .electric && hotWater == .electric {
            total += schedule.electricTotal
        } else {
            if heating == .electric { total += schedule.electricHeat }
            if hotWater == .electric { total += schedule.electricHotWater }
        }

        return total
    }
}

struct ApplicantPlan: Identifiable, Codable, Equatable {
    var id = UUID()
    var title: String
    var monthlyIncome: Double
    var monthlyRent: Double
    var voucherAmount: Double
    var notes: String
    var createdAt = Date()
    var program: VoucherProgram?
    var bedroomSize: BedroomSize?
    var zipCode: String?
    var annualGrossIncome: Double?
    var paymentStandard: Double?
    var utilityAllowance: Double?
    var contractRent: Double?
    var mixedFamily: Bool?
    var nonEligibleCount: Double?
    var currentlyInShelter: YesNoAnswer?
    var hasVoucher: YesNoAnswer?
    var incomeCountedForHousingConnect: Bool?

    var grossAnnualIncome: Double { annualGrossIncome ?? monthlyIncome * 12 }
    var grossMonthlyIncome: Double { grossAnnualIncome / 12 }
    var effectivePaymentStandard: Double { paymentStandard ?? voucherAmount }
    var effectiveUtilityAllowance: Double { utilityAllowance ?? 0 }
    var effectiveContractRent: Double { contractRent ?? monthlyRent }
    var rentPortion: Double { max(effectivePaymentStandard - effectiveUtilityAllowance, 0) }
    var contractRentAfterUtilities: Double? {
        guard let contractRent else { return nil }
        return max(contractRent - effectiveUtilityAllowance, 0)
    }
    var estimatedTenantShare: Double { grossMonthlyIncome * 0.3 }
    var estimatedSubsidy: Double { max(min(rentPortion, contractRentAfterUtilities ?? rentPortion), 0) }
    var tenantPortion: Double { estimatedTenantShare }
    var rentToIncomeRatio: Double { grossMonthlyIncome > 0 ? estimatedTenantShare / grossMonthlyIncome : 0 }
    var housingConnectIncomeStatus: String {
        incomeCountedForHousingConnect == false ? "Not counted for this Housing Connect screening path" : "Counted in calculator"
    }

    var summary: String {
        """
        HousingNav Applicant Plan
        Title: \(title)
        Program: \(program?.rawValue ?? "Manual")
        Bedroom size: \(bedroomSize?.rawValue ?? "Manual")
        Shelter answer: \(currentlyInShelter?.rawValue ?? "Not answered")
        Voucher answer: \(hasVoucher?.rawValue ?? "Not answered")
        Housing Connect income status: \(housingConnectIncomeStatus)
        Gross annual income: \(formatCurrency(grossAnnualIncome))
        Gross monthly income: \(formatCurrency(grossMonthlyIncome))
        Estimated tenant share: \(formatCurrency(estimatedTenantShare))
        Payment standard: \(formatCurrency(effectivePaymentStandard))
        Utility allowance: \(formatCurrency(effectiveUtilityAllowance))
        Rental portion after utilities: \(formatCurrency(rentPortion))
        Contract rent after utilities: \(contractRentAfterUtilities.map(formatCurrency) ?? "Not entered")
        Estimated max collectable subsidy rent: \(formatCurrency(estimatedSubsidy))
        Notes: \(notes.isEmpty ? "None" : notes)
        Planning note: HousingNav provides estimates and organization tools only. Final eligibility, rent approval, and lease-up decisions are made by the administering agency.
        """
    }
}

struct LandlordEstimate: Identifiable, Codable, Equatable {
    var id = UUID()
    var title: String
    var contractRent: Double
    var utilityAllowance: Double
    var requestedShare: Double
    var notes: String
    var createdAt = Date()
    var program: VoucherProgram?
    var bedroomSize: BedroomSize?
    var zipCode: String?
    var paymentStandard: Double?

    var rentPortion: Double { max(contractRent - utilityAllowance, 0) }
    var effectivePaymentStandard: Double { paymentStandard ?? grossRent }
    var grossRent: Double { contractRent + utilityAllowance }
    var maximumApprovableRentAfterUtilities: Double { max(effectivePaymentStandard - utilityAllowance, 0) }
    var voucherCoverage: Double { max(min(effectivePaymentStandard, rentPortion), 0) }
    var ownerCollectedAmount: Double { min(contractRent, requestedShare + voucherCoverage) }

    var summary: String {
        """
        HousingNav Landlord Estimate
        Title: \(title)
        Program: \(program?.rawValue ?? "Manual")
        Bedroom size: \(bedroomSize?.rawValue ?? "Manual")
        Contract rent: \(formatCurrency(contractRent))
        Rent portion after utilities: \(formatCurrency(rentPortion))
        Payment standard: \(formatCurrency(effectivePaymentStandard))
        Utility allowance: \(formatCurrency(utilityAllowance))
        Maximum approvable rent after utilities: \(formatCurrency(maximumApprovableRentAfterUtilities))
        Covered by voucher max: \(formatCurrency(voucherCoverage))
        Tenant share entered: \(formatCurrency(requestedShare))
        Estimated owner-collected total: \(formatCurrency(ownerCollectedAmount))
        Notes: \(notes.isEmpty ? "None" : notes)
        Planning note: HousingNav provides estimates and organization tools only. Final eligibility, rent approval, and lease-up decisions are made by the administering agency.
        """
    }
}

struct ChecklistItem: Identifiable, Codable, Equatable {
    var id = UUID()
    var role: AppRole
    var title: String
    var detail: String
    var isDone: Bool
}

struct ChatMessage: Identifiable, Codable, Equatable {
    enum Sender: String, Codable {
        case user
        case betty
    }

    var id = UUID()
    var sender: Sender
    var text: String
    var createdAt = Date()
}

struct SavedQuestion: Identifiable, Codable, Equatable {
    var id = UUID()
    var role: AppRole
    var text: String
    var createdAt = Date()
}

enum BettyTag: String, Codable, CaseIterable, Identifiable {
    case inspection = "Inspection"
    case documents = "Documents"
    case rent = "Rent"
    case voucher = "Voucher"
    case packet = "Packet"
    case general = "General"

    var id: String { rawValue }
}

struct SavedBettyAnswer: Identifiable, Codable, Equatable {
    var id = UUID()
    var role: AppRole
    var text: String
    var tag: BettyTag
    var createdAt = Date()
}

struct GuideProgressItem: Identifiable, Codable, Equatable {
    var id: String
    var role: AppRole
    var guideTitle: String
    var step: String
    var isDone: Bool
}

@MainActor
final class HousingNavStore: ObservableObject {
    @Published var applicantPlans: [ApplicantPlan] { didSet { save(applicantPlans, key: Keys.applicantPlans) } }
    @Published var landlordEstimates: [LandlordEstimate] { didSet { save(landlordEstimates, key: Keys.landlordEstimates) } }
    @Published var checklistItems: [ChecklistItem] { didSet { save(checklistItems, key: Keys.checklistItems) } }
    @Published var chatMessages: [ChatMessage] { didSet { save(chatMessages, key: Keys.chatMessages) } }
    @Published var savedQuestions: [SavedQuestion] { didSet { save(savedQuestions, key: Keys.savedQuestions) } }
    @Published var savedAnswers: [SavedBettyAnswer] { didSet { save(savedAnswers, key: Keys.savedAnswers) } }
    @Published var guideProgress: [GuideProgressItem] { didSet { save(guideProgress, key: Keys.guideProgress) } }
    @Published var favoriteListingIDs: Set<String> { didSet { save(Array(favoriteListingIDs), key: Keys.favoriteListingIDs) } }
    @Published var listingNotes: [String: String] { didSet { save(listingNotes, key: Keys.listingNotes) } }
    @Published var housingConnectListings: [HousingListing] { didSet { save(housingConnectListings, key: Keys.housingConnectListings) } }
    @Published var housingConnectFeedState: HousingConnectFeedState = .idle

    private enum Keys {
        static let applicantPlans = "housingNav.applicantPlans"
        static let landlordEstimates = "housingNav.landlordEstimates"
        static let checklistItems = "housingNav.checklistItems"
        static let chatMessages = "housingNav.chatMessages"
        static let savedQuestions = "housingNav.savedQuestions"
        static let savedAnswers = "housingNav.savedAnswers"
        static let guideProgress = "housingNav.guideProgress"
        static let favoriteListingIDs = "housingNav.favoriteListingIDs"
        static let listingNotes = "housingNav.listingNotes"
        static let housingConnectListings = "housingNav.housingConnectListings"
    }

    init(defaults: UserDefaults = .standard) {
        applicantPlans = Self.load(Keys.applicantPlans, defaults: defaults) ?? Self.sampleApplicantPlans
        landlordEstimates = Self.load(Keys.landlordEstimates, defaults: defaults) ?? Self.sampleLandlordEstimates
        checklistItems = Self.load(Keys.checklistItems, defaults: defaults) ?? Self.sampleChecklist
        chatMessages = Self.load(Keys.chatMessages, defaults: defaults) ?? Self.sampleMessages
        savedQuestions = Self.load(Keys.savedQuestions, defaults: defaults) ?? Self.sampleQuestions
        savedAnswers = Self.load(Keys.savedAnswers, defaults: defaults) ?? Self.sampleAnswers
        guideProgress = Self.load(Keys.guideProgress, defaults: defaults) ?? []
        favoriteListingIDs = Set(Self.load(Keys.favoriteListingIDs, defaults: defaults) ?? Self.sampleFavoriteListingIDs)
        listingNotes = Self.load(Keys.listingNotes, defaults: defaults) ?? Self.sampleListingNotes
        housingConnectListings = Self.load(Keys.housingConnectListings, defaults: defaults) ?? []
    }

    func saveApplicantPlan(_ plan: ApplicantPlan) {
        if let index = applicantPlans.firstIndex(where: { $0.id == plan.id }) {
            applicantPlans[index] = plan
        } else {
            applicantPlans.insert(plan, at: 0)
        }
    }

    func deleteApplicantPlan(_ plan: ApplicantPlan) {
        applicantPlans.removeAll { $0.id == plan.id }
    }

    func saveLandlordEstimate(_ estimate: LandlordEstimate) {
        if let index = landlordEstimates.firstIndex(where: { $0.id == estimate.id }) {
            landlordEstimates[index] = estimate
        } else {
            landlordEstimates.insert(estimate, at: 0)
        }
    }

    func deleteLandlordEstimate(_ estimate: LandlordEstimate) {
        landlordEstimates.removeAll { $0.id == estimate.id }
    }

    func toggleChecklistItem(_ item: ChecklistItem) {
        guard let index = checklistItems.firstIndex(where: { $0.id == item.id }) else { return }
        checklistItems[index].isDone.toggle()
    }

    func addChecklistItem(role: AppRole, title: String, detail: String) {
        let trimmedTitle = title.trimmingCharacters(in: .whitespacesAndNewlines)
        guard !trimmedTitle.isEmpty else { return }
        guard !checklistItems.contains(where: { $0.role == role && $0.title == trimmedTitle }) else { return }
        checklistItems.insert(ChecklistItem(role: role, title: trimmedTitle, detail: detail, isDone: false), at: 0)
    }

    func addBettyQuestion(_ text: String, role: AppRole) {
        let trimmed = text.trimmingCharacters(in: .whitespacesAndNewlines)
        guard !trimmed.isEmpty else { return }

        savedQuestions.insert(SavedQuestion(role: role, text: trimmed), at: 0)
        chatMessages.append(ChatMessage(sender: .user, text: trimmed))
        chatMessages.append(ChatMessage(sender: .betty, text: BettyResponseFactory.response(for: trimmed, role: role)))
    }

    func deleteSavedQuestion(_ question: SavedQuestion) {
        savedQuestions.removeAll { $0.id == question.id }
    }

    func saveBettyAnswer(_ text: String, role: AppRole, tag: BettyTag? = nil) {
        let trimmed = text.trimmingCharacters(in: .whitespacesAndNewlines)
        guard !trimmed.isEmpty else { return }
        guard !savedAnswers.contains(where: { $0.role == role && $0.text == trimmed }) else { return }
        savedAnswers.insert(SavedBettyAnswer(role: role, text: trimmed, tag: tag ?? Self.inferTag(for: trimmed, role: role)), at: 0)
    }

    func deleteSavedAnswer(_ answer: SavedBettyAnswer) {
        savedAnswers.removeAll { $0.id == answer.id }
    }

    func updateSavedAnswerTag(_ answer: SavedBettyAnswer, tag: BettyTag) {
        guard let index = savedAnswers.firstIndex(where: { $0.id == answer.id }) else { return }
        savedAnswers[index].tag = tag
    }

    func guideItemID(role: AppRole, guideTitle: String, step: String) -> String {
        "\(role.rawValue)|\(guideTitle)|\(step)"
    }

    func isGuideStepDone(role: AppRole, guideTitle: String, step: String) -> Bool {
        let id = guideItemID(role: role, guideTitle: guideTitle, step: step)
        return guideProgress.first(where: { $0.id == id })?.isDone ?? false
    }

    func toggleGuideStep(role: AppRole, guideTitle: String, step: String) {
        let id = guideItemID(role: role, guideTitle: guideTitle, step: step)
        if let index = guideProgress.firstIndex(where: { $0.id == id }) {
            guideProgress[index].isDone.toggle()
        } else {
            guideProgress.append(GuideProgressItem(id: id, role: role, guideTitle: guideTitle, step: step, isDone: true))
        }
    }

    func completedGuideSteps(role: AppRole, guideTitle: String, steps: [String]) -> Int {
        steps.filter { isGuideStepDone(role: role, guideTitle: guideTitle, step: $0) }.count
    }

    func isFavoriteListing(_ listing: HousingListing) -> Bool {
        favoriteListingIDs.contains(listing.id)
    }

    func toggleFavoriteListing(_ listing: HousingListing) {
        if favoriteListingIDs.contains(listing.id) {
            favoriteListingIDs.remove(listing.id)
        } else {
            favoriteListingIDs.insert(listing.id)
        }
    }

    func note(for listing: HousingListing) -> String {
        listingNotes[listing.id] ?? ""
    }

    func updateNote(_ note: String, for listing: HousingListing) {
        let trimmed = note.trimmingCharacters(in: .whitespacesAndNewlines)
        if trimmed.isEmpty {
            listingNotes.removeValue(forKey: listing.id)
        } else {
            listingNotes[listing.id] = note
        }
    }

    var favoriteListings: [HousingListing] {
        allListings.filter { favoriteListingIDs.contains($0.id) }
    }

    var allListings: [HousingListing] {
        housingConnectListings + ListingCatalog.all.filter { sample in
            !housingConnectListings.contains(where: { $0.id == sample.id })
        }
    }

    func loadHousingConnectListings(force: Bool = false) async {
        if !force, !housingConnectListings.isEmpty, case .loaded = housingConnectFeedState {
            return
        }

        housingConnectFeedState = .loading

        do {
            let today = Date.now.formatted(.iso8601.year().month().day())
            var components = URLComponents(string: "https://data.cityofnewyork.us/resource/vy5i-a666.json")!
            components.queryItems = [
                URLQueryItem(name: "$where", value: "lottery_status='Active' AND lottery_end_date >= '\(today)T00:00:00'"),
                URLQueryItem(name: "$order", value: "lottery_end_date ASC"),
                URLQueryItem(name: "$limit", value: "25")
            ]

            let (data, _) = try await URLSession.shared.data(from: components.url!)
            let decoded = try JSONDecoder().decode([HousingConnectLotteryRecord].self, from: data)
            let listings = decoded.compactMap { $0.toListing() }

            if listings.isEmpty {
                housingConnectFeedState = .failed("No active Housing Connect listings were available right now.")
            } else {
                housingConnectListings = listings
                housingConnectFeedState = .loaded(.now)
            }
        } catch {
            housingConnectFeedState = .failed("Housing Connect could not be refreshed right now. Saved listings and notes still work normally.")
        }
    }

    static func inferredTagName(for text: String, role: AppRole) -> String {
        inferTag(for: text, role: role).rawValue
    }

    private static func inferTag(for text: String, role: AppRole) -> BettyTag {
        let lower = text.lowercased()
        if lower.contains("inspection") || lower.contains("smoke") || lower.contains("repair") { return .inspection }
        if lower.contains("document") || lower.contains("pdf") || lower.contains("proof") || lower.contains("id") { return .documents }
        if lower.contains("rent") || lower.contains("payment") || lower.contains("standard") || lower.contains("utility") { return .rent }
        if lower.contains("voucher") || lower.contains("section 8") || lower.contains("cityfheps") || lower.contains("fheps") { return .voucher }
        if role == .landlord || lower.contains("packet") || lower.contains("lease") { return .packet }
        return .general
    }

    private func save<T: Encodable>(_ value: T, key: String) {
        guard let data = try? JSONEncoder().encode(value) else { return }
        UserDefaults.standard.set(data, forKey: key)
    }

    private static func load<T: Decodable>(_ key: String, defaults: UserDefaults) -> T? {
        guard let data = defaults.data(forKey: key) else { return nil }
        return try? JSONDecoder().decode(T.self, from: data)
    }

    private static let sampleApplicantPlans = [
        ApplicantPlan(
            title: "Review Sample: Bronx 2BR",
            monthlyIncome: 2850,
            monthlyRent: 2400,
            voucherAmount: 1800,
            notes: "Sample data included so App Review can test saved native results offline.",
            currentlyInShelter: .yes,
            hasVoucher: .yes,
            incomeCountedForHousingConnect: false
        )
    ]

    private static let sampleLandlordEstimates = [
        LandlordEstimate(
            title: "Review Sample: Packet Estimate",
            contractRent: 2600,
            utilityAllowance: 145,
            requestedShare: 690,
            notes: "Sample estimate for App Review testing."
        )
    ]

    private static let sampleChecklist = [
        ChecklistItem(role: .applicant, title: "Confirm household size", detail: "Match household members to the application and voucher paperwork.", isDone: true),
        ChecklistItem(role: .applicant, title: "Save income documents", detail: "Keep pay stubs, award letters, and benefit statements together.", isDone: false),
        ChecklistItem(role: .applicant, title: "Housing Connect profile check", detail: "Review contact info, household details, and document readiness.", isDone: false),
        ChecklistItem(role: .applicant, title: "Inspection readiness", detail: "Track repairs, utilities, and appointment details before move-in.", isDone: false),
        ChecklistItem(role: .landlord, title: "Lease packet draft", detail: "Prepare lease, rider, W-9, ownership proof, and direct deposit forms.", isDone: true),
        ChecklistItem(role: .landlord, title: "Utility allowance review", detail: "Record who pays heat, hot water, cooking gas, and electric.", isDone: false),
        ChecklistItem(role: .landlord, title: "Inspection prep", detail: "Check smoke detectors, windows, outlets, leaks, and access.", isDone: false),
        ChecklistItem(role: .landlord, title: "Rent reasonableness notes", detail: "Keep comparable rent notes and building details nearby.", isDone: false)
    ]

    private static let sampleMessages = [
        ChatMessage(sender: .betty, text: "Ask about vouchers, documents, inspections, and next steps. Save useful answers or add them directly to your checklist.")
    ]

    private static let sampleQuestions = [
        SavedQuestion(role: .applicant, text: "What should I prepare before a voucher inspection?"),
        SavedQuestion(role: .landlord, text: "What usually goes in a landlord packet?")
    ]

    private static let sampleAnswers = [
        SavedBettyAnswer(role: .applicant, text: "Keep ID, income documents, voucher paperwork, and deadline notes in one folder before applying.", tag: .documents),
        SavedBettyAnswer(role: .landlord, text: "Confirm lease packet, W-9, ownership proof, utility responsibility, and inspection readiness before submission.", tag: .packet)
    ]

    private static let sampleFavoriteListingIDs = ["bronx-garden-2br", "queens-sunny-1br"]

    private static let sampleListingNotes = [
        "bronx-garden-2br": "Ask Betty about inspection prep and compare against voucher payment standard.",
        "queens-sunny-1br": "Save documents before contacting landlord."
    ]
}

enum BettyResponseFactory {
    static func response(for question: String, role: AppRole) -> String {
        let lowercased = question.lowercased()

        if lowercased.contains("inspection") {
            return "Start with safety and access: smoke detectors, working utilities, leaks, windows, outlets, and clear access to every room. Save this question and pair it with your checklist before the appointment."
        }

        if lowercased.contains("calculator") || lowercased.contains("rent") {
            return "Use the native calculator to estimate the key numbers, then save the result. You can share the result summary from the saved card when you need to send it to someone."
        }

        if role == .landlord {
            return "For landlord prep, keep the packet, utility responsibility, inspection notes, and saved estimate together. HousingNav keeps those pieces available locally so you can return to them quickly."
        }

        return "For applicant prep, focus on documents, eligibility notes, Housing Connect status, voucher questions, and saved calculator results. I saved this question so you can come back to it later."
    }
}

enum AppTheme {
    static let ink = Color(red: 0.09, green: 0.16, blue: 0.23)
    static let blue = Color(red: 0.13, green: 0.43, blue: 0.78)
    static let sky = Color(red: 0.83, green: 0.92, blue: 0.98)
    static let mint = Color(red: 0.72, green: 0.89, blue: 0.81)
    static let sand = Color(red: 0.98, green: 0.93, blue: 0.84)
    static let card = Color.white.opacity(0.92)
}

struct OnboardingView: View {
    let onContinue: () -> Void

    var body: some View {
        ZStack {
            AppBackground()

            ScrollView {
                VStack(alignment: .leading, spacing: 22) {
                    VStack(alignment: .leading, spacing: 10) {
                        Label("Native housing workspace", systemImage: "checkmark.seal.fill")
                            .font(.caption.weight(.bold))
                            .foregroundStyle(AppTheme.blue)

                        Text("HousingNav")
                            .font(.system(size: 44, weight: .heavy, design: .rounded))
                            .foregroundStyle(AppTheme.ink)

                        Text("Choose your role, calculate key numbers, save checklists, ask Betty, and export summaries in one native workspace.")
                            .font(.headline)
                            .foregroundStyle(.secondary)
                    }
                    .padding(24)
                    .background(AppTheme.card, in: RoundedRectangle(cornerRadius: 32, style: .continuous))
                    .shadow(color: .black.opacity(0.08), radius: 18, y: 10)

                    RoleExplainerCard(
                        title: "Applicant / Tenant",
                        subtitle: "Plan voucher questions, save calculator results, track Housing Connect prep, and keep recent Betty questions."
                    )

                    RoleExplainerCard(
                        title: "Landlord",
                        subtitle: "Estimate rent numbers, prepare packet steps, save inspection notes, and export estimate summaries."
                    )

                    Button(action: onContinue) {
                        Text("Start in HousingNav")
                            .frame(maxWidth: .infinity)
                    }
                    .buttonStyle(PrimaryButtonStyle())
                }
                .padding(20)
            }
        }
        .navigationBarHidden(true)
    }
}

struct RoleSelectionView: View {
    let onSelect: (AppRole) -> Void

    var body: some View {
        ZStack {
            AppBackground()

            ScrollView {
                VStack(alignment: .leading, spacing: 20) {
                    VStack(alignment: .leading, spacing: 8) {
                        Text("Choose your housing path to enter HousingNav.")
                            .font(.system(size: 34, weight: .heavy, design: .rounded))
                            .foregroundStyle(AppTheme.ink)

                        Text("Applicants and landlords see different native tools, calculators, guides, checklists, and saved workspaces.")
                            .font(.subheadline)
                            .foregroundStyle(.secondary)
                    }

                    Button {
                        onSelect(.applicant)
                    } label: {
                        RoleCard(
                            role: .applicant,
                            title: "Applicant / Tenant",
                            subtitle: "Voucher prep, Housing Connect checklist, Betty, saved plans, and results."
                        )
                    }
                    .frame(maxWidth: .infinity)
                    .contentShape(Rectangle())
                    .buttonStyle(.plain)

                    Button {
                        onSelect(.landlord)
                    } label: {
                        RoleCard(
                            role: .landlord,
                            title: "Landlord",
                            subtitle: "Packet prep, rent estimate, Betty, saved estimates, and export tools."
                        )
                    }
                    .frame(maxWidth: .infinity)
                    .contentShape(Rectangle())
                    .buttonStyle(.plain)
                }
                .padding(20)
            }
        }
        .navigationTitle("HousingNav")
        .navigationBarTitleDisplayMode(.inline)
    }
}

struct ApplicantDashboardView: View {
    @EnvironmentObject private var store: HousingNavStore
    let onSwitchRole: () -> Void
    let onChooseRole: () -> Void

    var completedCount: Int {
        store.checklistItems.filter { $0.role == .applicant && $0.isDone }.count
    }

    var totalCount: Int {
        store.checklistItems.filter { $0.role == .applicant }.count
    }

    private var savedAnswerCount: Int {
        store.savedAnswers.filter { $0.role == .applicant }.count
    }

    private var hasSavedProgress: Bool {
        completedCount > 0 || !store.applicantPlans.isEmpty || savedAnswerCount > 0
    }

    var body: some View {
        DashboardShell(title: "Applicant Hub", subtitle: "Track your progress, calculations, and next steps") {
            RoleSwitcherCard(
                currentRole: .applicant,
                onSwitchRole: onSwitchRole,
                onChooseRole: onChooseRole
            )

            ProgressHero(title: "Checklist progress", value: completedCount, total: totalCount)

            VStack(alignment: .leading, spacing: 10) {
                Text(hasSavedProgress ? "Continue where you left off" : "Start your first task")
                    .font(.headline)
                    .foregroundStyle(AppTheme.ink)
                CompactCard(
                    symbol: hasSavedProgress ? "arrow.triangle.2.circlepath" : "sparkles",
                    title: hasSavedProgress ? "\(max(totalCount - completedCount, 0)) checklist steps left" : "Open the calculator or checklist",
                    subtitle: hasSavedProgress ? "\(store.applicantPlans.count) saved plan\(store.applicantPlans.count == 1 ? "" : "s") and \(savedAnswerCount) saved Betty answer\(savedAnswerCount == 1 ? "" : "s")." : "HousingNav will save your first result, checklist step, or Betty answer locally."
                )
            }

            DashboardGrid {
                NavigationLink {
                    BettyChatView(role: .applicant)
                } label: {
                    DashboardCard(symbol: "sparkles", title: "Betty assistant", subtitle: "Ask native prompts and save questions.")
                }

                NavigationLink {
                    VoucherHelpView()
                } label: {
                    DashboardCard(symbol: "doc.text.magnifyingglass", title: "Voucher Help", subtitle: "Checklist-ready voucher actions.")
                }

                NavigationLink {
                    ApplicantCalculatorView()
                } label: {
                    DashboardCard(symbol: "function", title: "Calculator", subtitle: "Estimate tenant portion and save.")
                }

                NavigationLink {
                    ChecklistView(role: .applicant)
                } label: {
                    DashboardCard(symbol: "checklist", title: "Housing Connect checklist", subtitle: "\(completedCount) of \(totalCount) complete.")
                }

                NavigationLink {
                    SavedApplicantPlansView()
                } label: {
                    DashboardCard(symbol: "tray.full", title: "Saved plans/results", subtitle: "\(store.applicantPlans.count) saved result\(store.applicantPlans.count == 1 ? "" : "s").")
                }

                NavigationLink {
                    SavedAnswersView(role: .applicant)
                } label: {
                    DashboardCard(symbol: "bookmark.circle", title: "Saved Betty answers", subtitle: "\(savedAnswerCount) tagged answer\(savedAnswerCount == 1 ? "" : "s").")
                }

                NavigationLink {
                    ResourcesView(role: .applicant)
                } label: {
                    DashboardCard(symbol: "book.closed", title: "Native guides", subtitle: "Housing Connect prep, lottery steps, and voucher basics.")
                }
            }
        }
        .navigationTitle("Applicant")
        .navigationBarTitleDisplayMode(.inline)
        .toolbar {
            Menu {
                Button("Switch to Landlord", action: onSwitchRole)
                Button("Choose Route", action: onChooseRole)
            } label: {
                Label("Roles", systemImage: "arrow.left.arrow.right.circle")
            }
        }
    }
}

struct LandlordDashboardView: View {
    @EnvironmentObject private var store: HousingNavStore
    let onSwitchRole: () -> Void
    let onChooseRole: () -> Void

    var completedCount: Int {
        store.checklistItems.filter { $0.role == .landlord && $0.isDone }.count
    }

    var totalCount: Int {
        store.checklistItems.filter { $0.role == .landlord }.count
    }

    private var savedAnswerCount: Int {
        store.savedAnswers.filter { $0.role == .landlord }.count
    }

    private var hasSavedProgress: Bool {
        completedCount > 0 || !store.landlordEstimates.isEmpty || savedAnswerCount > 0
    }

    var body: some View {
        DashboardShell(title: "Landlord Hub", subtitle: "Manage estimates, packet prep, and inspection steps") {
            RoleSwitcherCard(
                currentRole: .landlord,
                onSwitchRole: onSwitchRole,
                onChooseRole: onChooseRole
            )

            ProgressHero(title: "Packet prep progress", value: completedCount, total: totalCount)

            VStack(alignment: .leading, spacing: 10) {
                Text(hasSavedProgress ? "Continue where you left off" : "Start your first task")
                    .font(.headline)
                    .foregroundStyle(AppTheme.ink)
                CompactCard(
                    symbol: hasSavedProgress ? "arrow.triangle.2.circlepath" : "sparkles",
                    title: hasSavedProgress ? "\(max(totalCount - completedCount, 0)) packet steps left" : "Open an estimate or packet checklist",
                    subtitle: hasSavedProgress ? "\(store.landlordEstimates.count) saved estimate\(store.landlordEstimates.count == 1 ? "" : "s") and \(savedAnswerCount) saved Betty answer\(savedAnswerCount == 1 ? "" : "s")." : "HousingNav will save your first estimate, packet step, or Betty answer locally."
                )
            }

            DashboardGrid {
                NavigationLink {
                    LandlordCalculatorView()
                } label: {
                    DashboardCard(symbol: "building.columns", title: "Landlord calculator", subtitle: "Estimate gross and owner-collected rent.")
                }

                NavigationLink {
                    ChecklistView(role: .landlord)
                } label: {
                    DashboardCard(symbol: "folder.badge.gearshape", title: "Packet prep checklist", subtitle: "\(completedCount) of \(totalCount) complete.")
                }

                NavigationLink {
                    BettyChatView(role: .landlord)
                } label: {
                    DashboardCard(symbol: "sparkles", title: "Betty assistant", subtitle: "Ask packet and inspection questions.")
                }

                NavigationLink {
                    SavedLandlordEstimatesView()
                } label: {
                    DashboardCard(symbol: "archivebox", title: "Saved estimates", subtitle: "\(store.landlordEstimates.count) saved estimate\(store.landlordEstimates.count == 1 ? "" : "s").")
                }

                NavigationLink {
                    SavedAnswersView(role: .landlord)
                } label: {
                    DashboardCard(symbol: "bookmark.circle", title: "Saved Betty answers", subtitle: "\(savedAnswerCount) tagged answer\(savedAnswerCount == 1 ? "" : "s").")
                }

                NavigationLink {
                    ResourcesView(role: .landlord)
                } label: {
                    DashboardCard(symbol: "building.2", title: "Landlord resources", subtitle: "Packet prep, lease-up FAQ, inspections, and program notes.")
                }
            }
        }
        .navigationTitle("Landlord")
        .navigationBarTitleDisplayMode(.inline)
        .toolbar {
            Menu {
                Button("Switch to Applicant / Tenant", action: onSwitchRole)
                Button("Choose Route", action: onChooseRole)
            } label: {
                Label("Roles", systemImage: "arrow.left.arrow.right.circle")
            }
        }
    }
}

struct BettyChatView: View {
    @EnvironmentObject private var store: HousingNavStore
    let role: AppRole
    @State private var draft = ""

    private var roleQuestions: [SavedQuestion] {
        store.savedQuestions.filter { $0.role == role }
    }

    var body: some View {
        VStack(spacing: 0) {
            ScrollView {
                VStack(alignment: .leading, spacing: 16) {
                    StarterPromptStrip(role: role) { prompt in
                        draft = prompt
                    }

                    if store.chatMessages.isEmpty {
                        EmptyStateView(
                            symbol: "bubble.left.and.bubble.right",
                            title: "No conversation yet",
                            message: "Ask about vouchers, documents, inspections, and next steps. Save useful answers or add them directly to your checklist."
                        )
                    } else {
                        ForEach(store.chatMessages) { message in
                            ChatBubble(message: message, role: role)
                        }
                    }

                    if !roleQuestions.isEmpty {
                        VStack(alignment: .leading, spacing: 8) {
                            Text("Saved recent questions")
                                .font(.headline)
                            ForEach(roleQuestions.prefix(5)) { question in
                                Button {
                                    draft = question.text
                                } label: {
                                    CompactCard(symbol: "clock.arrow.circlepath", title: question.text, subtitle: "Tap to ask again")
                                }
                                .buttonStyle(.plain)
                            }
                        }
                    }
                }
                .padding()
            }

            HStack(spacing: 10) {
                TextField("Ask Betty about \(role.shortTitle.lowercased()) steps", text: $draft, axis: .vertical)
                    .textFieldStyle(.roundedBorder)
                    .lineLimit(1...4)

                Button {
                    store.addBettyQuestion(draft, role: role)
                    draft = ""
                } label: {
                    Image(systemName: "arrow.up.circle.fill")
                        .font(.system(size: 34))
                }
                .disabled(draft.trimmingCharacters(in: .whitespacesAndNewlines).isEmpty)
            }
            .padding()
            .background(.regularMaterial)
        }
        .navigationTitle("Betty")
        .navigationBarTitleDisplayMode(.inline)
        .toolbar {
            NavigationLink {
                SavedAnswersView(role: role)
            } label: {
                Label("Saved answers", systemImage: "bookmark")
            }
        }
    }
}

struct ApplicantCalculatorView: View {
    @EnvironmentObject private var store: HousingNavStore
    @Environment(\.dismiss) private var dismiss
    let existingPlan: ApplicantPlan?

    @State private var title: String
    @State private var program: VoucherProgram
    @State private var bedroomSize: BedroomSize
    @State private var zipCode: String
    @State private var utilityShortcut: UtilityShortcut
    @State private var cooking: CookingUtility
    @State private var otherElectric: OtherElectricUtility
    @State private var section8HeatHotWater: Section8HeatHotWater
    @State private var heating: HeatUtility
    @State private var hotWater: HeatUtility
    @State private var currentlyInShelter: YesNoAnswer
    @State private var hasVoucher: YesNoAnswer
    @State private var annualGrossIncome: String
    @State private var contractRent: String
    @State private var mixedFamily: Bool
    @State private var nonEligibleCount: String
    @State private var notes: String
    @State private var showValidation = false

    init(existingPlan: ApplicantPlan? = nil) {
        self.existingPlan = existingPlan
        _title = State(initialValue: existingPlan?.title ?? "")
        _program = State(initialValue: existingPlan?.program ?? .section8)
        _bedroomSize = State(initialValue: existingPlan?.bedroomSize ?? .two)
        _zipCode = State(initialValue: existingPlan?.zipCode ?? "")
        _utilityShortcut = State(initialValue: .landlordPaysAll)
        _cooking = State(initialValue: .none)
        _otherElectric = State(initialValue: .no)
        _section8HeatHotWater = State(initialValue: .none)
        _heating = State(initialValue: .none)
        _hotWater = State(initialValue: .none)
        _currentlyInShelter = State(initialValue: existingPlan?.currentlyInShelter ?? .notAnswered)
        _hasVoucher = State(initialValue: existingPlan?.hasVoucher ?? .notAnswered)
        _annualGrossIncome = State(initialValue: existingPlan.map { String(format: "%.0f", $0.grossAnnualIncome) } ?? "")
        _contractRent = State(initialValue: existingPlan?.contractRent.map { String(format: "%.0f", $0) } ?? "")
        _mixedFamily = State(initialValue: existingPlan?.mixedFamily ?? false)
        _nonEligibleCount = State(initialValue: existingPlan?.nonEligibleCount.map { String(format: "%.0f", $0) } ?? "")
        _notes = State(initialValue: existingPlan?.notes ?? "")
    }

    private var paymentStandard: Double {
        HousingCalculatorEngine.paymentStandard(program: program, bedroom: bedroomSize, zipCode: zipCode)
    }

    private var utilityAllowance: Double {
        if program == .section8 {
            HousingCalculatorEngine.section8UtilityAllowance(
                bedroom: bedroomSize,
                cooking: cooking,
                otherElectric: otherElectric,
                heatHotWater: section8HeatHotWater
            )
        } else {
            HousingCalculatorEngine.nonSection8UtilityAllowance(
                program: program,
                bedroom: bedroomSize,
                cooking: cooking,
                otherElectric: otherElectric,
                heating: heating,
                hotWater: hotWater
            )
        }
    }

    private var incomeIsCountedForHousingConnect: Bool {
        !(currentlyInShelter == .yes && hasVoucher != .notAnswered)
    }

    private var effectiveAnnualIncome: Double? {
        if incomeIsCountedForHousingConnect {
            guard let income = Double(annualGrossIncome), income > 0 else { return nil }
            return income
        }
        return 0
    }

    private var incomeStatusText: String {
        if incomeIsCountedForHousingConnect {
            return "Income is counted in this calculator. Enter the household's gross annual income."
        }
        return "Income entry is disabled for this shelter/voucher Housing Connect screening path."
    }

    private var parsedPlan: ApplicantPlan? {
        guard let income = effectiveAnnualIncome else { return nil }

        let enteredRent = Double(contractRent)

        return ApplicantPlan(
            id: existingPlan?.id ?? UUID(),
            title: title.isEmpty ? "\(program.rawValue) \(bedroomSize.rawValue) plan" : title,
            monthlyIncome: income / 12,
            monthlyRent: enteredRent ?? paymentStandard,
            voucherAmount: paymentStandard,
            notes: notes,
            createdAt: existingPlan?.createdAt ?? Date(),
            program: program,
            bedroomSize: bedroomSize,
            zipCode: zipCode,
            annualGrossIncome: income,
            paymentStandard: paymentStandard,
            utilityAllowance: utilityAllowance,
            contractRent: enteredRent,
            mixedFamily: mixedFamily,
            nonEligibleCount: Double(nonEligibleCount),
            currentlyInShelter: currentlyInShelter,
            hasVoucher: hasVoucher,
            incomeCountedForHousingConnect: incomeIsCountedForHousingConnect
        )
    }

    var body: some View {
        Form {
            Section("Voucher setup") {
                Picker("Program", selection: $program) {
                    ForEach(VoucherProgram.allCases) { program in
                        Text(program.rawValue).tag(program)
                    }
                }
                Picker("Voucher or unit bedroom size", selection: $bedroomSize) {
                    ForEach(BedroomSize.allCases) { size in
                        Text(size.rawValue).tag(size)
                    }
                }
                if program == .section8 {
                    TextField("ZIP code for exception standard lookup", text: $zipCode)
                        .keyboardType(.numberPad)
                }
            }

            Section("Utility setup") {
                Picker("Shortcut", selection: $utilityShortcut) {
                    ForEach(UtilityShortcut.allCases) { shortcut in
                        Text(shortcut.rawValue).tag(shortcut)
                    }
                }
                Button("Apply utility shortcut") {
                    applyUtilityShortcut()
                }

                Picker("Cooking utility", selection: $cooking) {
                    ForEach(CookingUtility.allCases) { utility in
                        Text(utility.rawValue).tag(utility)
                    }
                }
                Picker("Other electric service", selection: $otherElectric) {
                    ForEach(OtherElectricUtility.allCases) { utility in
                        Text(utility.rawValue).tag(utility)
                    }
                }

                if program == .section8 {
                    Picker("Heat and hot water", selection: $section8HeatHotWater) {
                        ForEach(Section8HeatHotWater.allCases) { utility in
                            Text(utility.rawValue).tag(utility)
                        }
                    }
                } else {
                    Picker("Heating utility", selection: $heating) {
                        ForEach(HeatUtility.allCases) { utility in
                            Text(utility.rawValue).tag(utility)
                        }
                    }
                    Picker("Hot water utility", selection: $hotWater) {
                        ForEach(HeatUtility.allCases) { utility in
                            Text(utility.rawValue).tag(utility)
                        }
                    }
                }

                LabeledContent("Payment standard", value: formatCurrency(paymentStandard))
                LabeledContent("Utility allowance", value: formatCurrency(utilityAllowance))
            }

            Section("Housing Connect eligibility screen") {
                Picker("Are you currently in shelter?", selection: $currentlyInShelter) {
                    ForEach(YesNoAnswer.allCases) { answer in
                        Text(answer.rawValue).tag(answer)
                    }
                }

                Picker("Do you have a voucher or rental subsidy?", selection: $hasVoucher) {
                    ForEach(YesNoAnswer.allCases) { answer in
                        Text(answer.rawValue).tag(answer)
                    }
                }

                Label(incomeStatusText, systemImage: incomeIsCountedForHousingConnect ? "person.text.rectangle" : "checkmark.seal.fill")
                    .font(.footnote)
                    .foregroundStyle(incomeIsCountedForHousingConnect ? Color.secondary : Color.green)
            }

            Section("Tenant inputs") {
                MoneyField(title: "Gross annual household income", text: $annualGrossIncome)
                    .disabled(!incomeIsCountedForHousingConnect)
                    .foregroundStyle(incomeIsCountedForHousingConnect ? .primary : .secondary)
                MoneyField(title: "Contract rent (optional)", text: $contractRent)
                Toggle("Mixed family / non-eligible citizen household", isOn: $mixedFamily)
                if mixedFamily {
                    TextField("Number of non-eligible citizens", text: $nonEligibleCount)
                        .keyboardType(.numberPad)
                }
                TextField("Notes", text: $notes, axis: .vertical)
                    .lineLimit(3...6)
            }

            if let plan = parsedPlan {
                Section("Native result") {
                    ResultCard(
                        title: plan.title,
                        rows: [
                            ("Housing Connect income status", plan.housingConnectIncomeStatus),
                            ("Gross monthly household income", formatCurrency(plan.grossMonthlyIncome)),
                            ("Estimated tenant share (30%)", formatCurrency(plan.estimatedTenantShare)),
                            ("Rental portion after utilities", formatCurrency(plan.rentPortion)),
                            ("Contract rent after utilities", plan.contractRentAfterUtilities.map(formatCurrency) ?? "Not entered"),
                            ("Estimated max collectable subsidy rent", formatCurrency(plan.estimatedSubsidy))
                        ],
                        shareText: plan.summary
                    )
                    Text(program.note)
                        .font(.footnote)
                        .foregroundStyle(.secondary)
                    Text("HousingNav provides planning estimates and local organization tools only. Final eligibility, rent approval, and lease-up decisions are made by the administering agency.")
                        .font(.footnote)
                        .foregroundStyle(.secondary)
                    if mixedFamily {
                        Text("Mixed family disclaimer: the final subsidy may be adjusted by the housing authority based on eligible family members.")
                            .font(.footnote)
                            .foregroundStyle(.orange)
                    }
                }
            } else if showValidation {
                Section {
                    Label(incomeIsCountedForHousingConnect ? "Enter valid gross annual income before saving." : "Answer the shelter and voucher questions before saving.", systemImage: "exclamationmark.triangle.fill")
                        .foregroundStyle(.orange)
                }
            }

            Section {
                Button(existingPlan == nil ? "Save result" : "Update result") {
                    guard let plan = parsedPlan else {
                        showValidation = true
                        return
                    }
                    store.saveApplicantPlan(plan)
                    dismiss()
                }
            }
        }
        .navigationTitle(existingPlan == nil ? "Tenant Calculator" : "Edit Result")
        .onChange(of: currentlyInShelter) { _, _ in
            if !incomeIsCountedForHousingConnect {
                annualGrossIncome = ""
            }
        }
        .onChange(of: hasVoucher) { _, _ in
            if !incomeIsCountedForHousingConnect {
                annualGrossIncome = ""
            }
        }
    }

    private func applyUtilityShortcut() {
        switch utilityShortcut {
        case .landlordPaysAll:
            cooking = .none
            otherElectric = .no
            section8HeatHotWater = .none
            heating = .none
            hotWater = .none
        case .tenantPaysAllGas:
            cooking = .gas
            otherElectric = .yes
            section8HeatHotWater = .gas
            heating = .gas
            hotWater = .gas
        case .tenantPaysAllElectric:
            cooking = .electric
            otherElectric = .yes
            section8HeatHotWater = .electric
            heating = .electric
            hotWater = .electric
        case .custom:
            break
        }
    }
}

struct LandlordCalculatorView: View {
    @EnvironmentObject private var store: HousingNavStore
    @Environment(\.dismiss) private var dismiss
    let existingEstimate: LandlordEstimate?

    @State private var title: String
    @State private var program: VoucherProgram
    @State private var bedroomSize: BedroomSize
    @State private var zipCode: String
    @State private var utilityShortcut: UtilityShortcut
    @State private var cooking: CookingUtility
    @State private var otherElectric: OtherElectricUtility
    @State private var section8HeatHotWater: Section8HeatHotWater
    @State private var heating: HeatUtility
    @State private var hotWater: HeatUtility
    @State private var contractRent: String
    @State private var requestedShare: String
    @State private var notes: String
    @State private var showValidation = false

    init(existingEstimate: LandlordEstimate? = nil) {
        self.existingEstimate = existingEstimate
        _title = State(initialValue: existingEstimate?.title ?? "")
        _program = State(initialValue: existingEstimate?.program ?? .section8)
        _bedroomSize = State(initialValue: existingEstimate?.bedroomSize ?? .two)
        _zipCode = State(initialValue: existingEstimate?.zipCode ?? "")
        _utilityShortcut = State(initialValue: .landlordPaysAll)
        _cooking = State(initialValue: .none)
        _otherElectric = State(initialValue: .no)
        _section8HeatHotWater = State(initialValue: .none)
        _heating = State(initialValue: .none)
        _hotWater = State(initialValue: .none)
        _contractRent = State(initialValue: existingEstimate.map { String(format: "%.0f", $0.contractRent) } ?? "")
        _requestedShare = State(initialValue: existingEstimate.map { String(format: "%.0f", $0.requestedShare) } ?? "")
        _notes = State(initialValue: existingEstimate?.notes ?? "")
    }

    private var paymentStandard: Double {
        HousingCalculatorEngine.paymentStandard(program: program, bedroom: bedroomSize, zipCode: zipCode)
    }

    private var utilityAllowance: Double {
        if program == .section8 {
            HousingCalculatorEngine.section8UtilityAllowance(
                bedroom: bedroomSize,
                cooking: cooking,
                otherElectric: otherElectric,
                heatHotWater: section8HeatHotWater
            )
        } else {
            HousingCalculatorEngine.nonSection8UtilityAllowance(
                program: program,
                bedroom: bedroomSize,
                cooking: cooking,
                otherElectric: otherElectric,
                heating: heating,
                hotWater: hotWater
            )
        }
    }

    private var parsedEstimate: LandlordEstimate? {
        guard
            let rent = Double(contractRent),
            let share = Double(requestedShare.isEmpty ? "0" : requestedShare),
            rent > 0,
            share >= 0
        else { return nil }

        return LandlordEstimate(
            id: existingEstimate?.id ?? UUID(),
            title: title.isEmpty ? "\(program.rawValue) \(bedroomSize.rawValue) estimate" : title,
            contractRent: rent,
            utilityAllowance: utilityAllowance,
            requestedShare: share,
            notes: notes,
            createdAt: existingEstimate?.createdAt ?? Date(),
            program: program,
            bedroomSize: bedroomSize,
            zipCode: zipCode,
            paymentStandard: paymentStandard
        )
    }

    var body: some View {
        Form {
            Section("Program setup") {
                Picker("Program", selection: $program) {
                    ForEach(VoucherProgram.allCases) { program in
                        Text(program.rawValue).tag(program)
                    }
                }
                Picker("Voucher or unit bedroom size", selection: $bedroomSize) {
                    ForEach(BedroomSize.allCases) { size in
                        Text(size.rawValue).tag(size)
                    }
                }
                if program == .section8 {
                    TextField("ZIP code for exception standard lookup", text: $zipCode)
                        .keyboardType(.numberPad)
                }
            }

            Section("Utility setup") {
                Picker("Shortcut", selection: $utilityShortcut) {
                    ForEach(UtilityShortcut.allCases) { shortcut in
                        Text(shortcut.rawValue).tag(shortcut)
                    }
                }
                Button("Apply utility shortcut") {
                    applyUtilityShortcut()
                }

                Picker("Cooking utility", selection: $cooking) {
                    ForEach(CookingUtility.allCases) { utility in
                        Text(utility.rawValue).tag(utility)
                    }
                }
                Picker("Other electric service", selection: $otherElectric) {
                    ForEach(OtherElectricUtility.allCases) { utility in
                        Text(utility.rawValue).tag(utility)
                    }
                }

                if program == .section8 {
                    Picker("Heat and hot water", selection: $section8HeatHotWater) {
                        ForEach(Section8HeatHotWater.allCases) { utility in
                            Text(utility.rawValue).tag(utility)
                        }
                    }
                } else {
                    Picker("Heating utility", selection: $heating) {
                        ForEach(HeatUtility.allCases) { utility in
                            Text(utility.rawValue).tag(utility)
                        }
                    }
                    Picker("Hot water utility", selection: $hotWater) {
                        ForEach(HeatUtility.allCases) { utility in
                            Text(utility.rawValue).tag(utility)
                        }
                    }
                }

                LabeledContent("Payment standard", value: formatCurrency(paymentStandard))
                LabeledContent("Utility allowance", value: formatCurrency(utilityAllowance))
            }

            Section("Rent inputs") {
                MoneyField(title: "Contract rent", text: $contractRent)
                MoneyField(title: "Tenant share, if known", text: $requestedShare)
                TextField("Notes", text: $notes, axis: .vertical)
                    .lineLimit(3...6)
            }

            if let estimate = parsedEstimate {
                Section("Native result") {
                    ResultCard(
                        title: estimate.title,
                        rows: [
                            ("Rent portion after utilities", formatCurrency(estimate.rentPortion)),
                            ("Payment standard", formatCurrency(estimate.effectivePaymentStandard)),
                            ("Maximum approvable rent after utilities", formatCurrency(estimate.maximumApprovableRentAfterUtilities)),
                            ("Covered by voucher max", formatCurrency(estimate.voucherCoverage))
                        ],
                        shareText: estimate.summary
                    )
                    Text(program.note)
                        .font(.footnote)
                        .foregroundStyle(.secondary)
                    Text("HousingNav provides planning estimates and local organization tools only. Final eligibility, rent approval, and lease-up decisions are made by the administering agency.")
                        .font(.footnote)
                        .foregroundStyle(.secondary)
                }
            } else if showValidation {
                Section {
                    Label("Enter valid contract rent before saving.", systemImage: "exclamationmark.triangle.fill")
                        .foregroundStyle(.orange)
                }
            }

            Section {
                Button(existingEstimate == nil ? "Save estimate" : "Update estimate") {
                    guard let estimate = parsedEstimate else {
                        showValidation = true
                        return
                    }
                    store.saveLandlordEstimate(estimate)
                    dismiss()
                }
            }
        }
        .navigationTitle(existingEstimate == nil ? "Landlord Calculator" : "Edit Estimate")
    }

    private func applyUtilityShortcut() {
        switch utilityShortcut {
        case .landlordPaysAll:
            cooking = .none
            otherElectric = .no
            section8HeatHotWater = .none
            heating = .none
            hotWater = .none
        case .tenantPaysAllGas:
            cooking = .gas
            otherElectric = .yes
            section8HeatHotWater = .gas
            heating = .gas
            hotWater = .gas
        case .tenantPaysAllElectric:
            cooking = .electric
            otherElectric = .yes
            section8HeatHotWater = .electric
            heating = .electric
            hotWater = .electric
        case .custom:
            break
        }
    }
}

struct ChecklistView: View {
    @EnvironmentObject private var store: HousingNavStore
    let role: AppRole
    @State private var confirmation: String?

    private var items: [ChecklistItem] {
        store.checklistItems.filter { $0.role == role }
    }

    private var completedCount: Int {
        items.filter(\.isDone).count
    }

    var body: some View {
        List {
            Section {
                VStack(alignment: .leading, spacing: 10) {
                    ProgressView(value: Double(completedCount), total: Double(max(items.count, 1)))
                    HStack {
                        Text("\(completedCount)/\(items.count) steps complete")
                            .font(.subheadline.weight(.semibold))
                            .foregroundStyle(AppTheme.ink)
                        Spacer()
                        if let confirmation {
                            Label(confirmation, systemImage: "checkmark.circle.fill")
                                .font(.caption.weight(.semibold))
                                .foregroundStyle(.green)
                        }
                    }
                }
                .padding(.vertical, 4)
            }

            ForEach(items) { item in
                Button {
                    store.toggleChecklistItem(item)
                    confirmation = item.isDone ? "Marked incomplete" : "Marked complete"
                } label: {
                    HStack(alignment: .top, spacing: 12) {
                        Image(systemName: item.isDone ? "checkmark.circle.fill" : "circle")
                            .foregroundStyle(item.isDone ? .green : .secondary)
                            .font(.title3)

                        VStack(alignment: .leading, spacing: 4) {
                            Text(item.title)
                                .font(.headline)
                                .foregroundStyle(AppTheme.ink)
                            Text(item.detail)
                                .font(.subheadline)
                                .foregroundStyle(.secondary)
                        }
                    }
                    .padding(.vertical, 4)
                }
                .buttonStyle(.plain)
            }
        }
        .overlay {
            if items.isEmpty {
                EmptyStateView(symbol: "checklist", title: "No checklist items", message: "Checklist progress is saved locally once items are added.")
            }
        }
        .navigationTitle(role == .applicant ? "Housing Checklist" : "Packet Checklist")
    }
}

struct SavedApplicantPlansView: View {
    @EnvironmentObject private var store: HousingNavStore

    var body: some View {
        List {
            ForEach(store.applicantPlans) { plan in
                NavigationLink {
                    ApplicantCalculatorView(existingPlan: plan)
                } label: {
                    SavedResultRow(title: plan.title, subtitle: "Tenant portion \(formatCurrency(plan.tenantPortion))", createdAt: plan.createdAt)
                }
                .swipeActions {
                    Button(role: .destructive) {
                        store.deleteApplicantPlan(plan)
                    } label: {
                        Label("Delete", systemImage: "trash")
                    }
                }
            }
        }
        .overlay {
            if store.applicantPlans.isEmpty {
                EmptyStateView(symbol: "tray", title: "No saved plans yet", message: "Run the applicant calculator and save a result to build a local plan history.")
            }
        }
        .toolbar {
            NavigationLink {
                ApplicantCalculatorView()
            } label: {
                Image(systemName: "plus")
            }
        }
        .navigationTitle("Saved Plans")
    }
}

struct SavedLandlordEstimatesView: View {
    @EnvironmentObject private var store: HousingNavStore

    var body: some View {
        List {
            ForEach(store.landlordEstimates) { estimate in
                NavigationLink {
                    LandlordCalculatorView(existingEstimate: estimate)
                } label: {
                    SavedResultRow(title: estimate.title, subtitle: "Gross rent \(formatCurrency(estimate.grossRent))", createdAt: estimate.createdAt)
                }
                .swipeActions {
                    Button(role: .destructive) {
                        store.deleteLandlordEstimate(estimate)
                    } label: {
                        Label("Delete", systemImage: "trash")
                    }
                }
            }
        }
        .overlay {
            if store.landlordEstimates.isEmpty {
                EmptyStateView(symbol: "archivebox", title: "No saved estimates yet", message: "Run the landlord calculator and save an estimate for local review.")
            }
        }
        .toolbar {
            NavigationLink {
                LandlordCalculatorView()
            } label: {
                Image(systemName: "plus")
            }
        }
        .navigationTitle("Saved Estimates")
    }
}

struct SavedAnswersView: View {
    @EnvironmentObject private var store: HousingNavStore
    let role: AppRole
    @State private var confirmation: String?

    private var answers: [SavedBettyAnswer] {
        store.savedAnswers.filter { $0.role == role }
    }

    var body: some View {
        List {
            ForEach(answers) { answer in
                VStack(alignment: .leading, spacing: 10) {
                    Text(answer.text)
                        .font(.body)
                        .foregroundStyle(AppTheme.ink)

                    Picker("Tag", selection: Binding(
                        get: { answer.tag },
                        set: { store.updateSavedAnswerTag(answer, tag: $0) }
                    )) {
                        ForEach(BettyTag.allCases) { tag in
                            Text(tag.rawValue).tag(tag)
                        }
                    }
                    .pickerStyle(.menu)

                    Button {
                        store.addChecklistItem(role: role, title: answer.tag.rawValue, detail: answer.text)
                        confirmation = "Added to checklist"
                    } label: {
                        Label("Add to checklist", systemImage: "checklist")
                    }
                    .buttonStyle(.bordered)
                }
                .padding(.vertical, 6)
                .swipeActions {
                    Button(role: .destructive) {
                        store.deleteSavedAnswer(answer)
                    } label: {
                        Label("Delete", systemImage: "trash")
                    }
                }
            }
        }
        .overlay {
            if answers.isEmpty {
                EmptyStateView(symbol: "bookmark", title: "No saved answers yet", message: "Save a Betty response to reuse it, tag it, or add it to your checklist.")
            }
        }
        .safeAreaInset(edge: .bottom) {
            if let confirmation {
                Label(confirmation, systemImage: "checkmark.circle.fill")
                    .font(.caption.weight(.semibold))
                    .foregroundStyle(.green)
                    .padding(.horizontal, 14)
                    .padding(.vertical, 10)
                    .background(.regularMaterial, in: Capsule())
                    .padding(.bottom, 8)
            }
        }
        .navigationTitle("Saved Answers")
    }
}

struct ListingsView: View {
    @EnvironmentObject private var store: HousingNavStore
    @State private var maxPrice = ""
    @State private var minimumBedrooms = 0
    @State private var locationText = ""
    @State private var selectedSource: ListingSource?
    @State private var selectedBorough = "All boroughs"
    @State private var sortMode = ListingSortMode.closingSoonest
    @State private var mapPosition = MapCameraPosition.region(
        MKCoordinateRegion(
            center: CLLocationCoordinate2D(latitude: 40.7580, longitude: -73.9855),
            span: MKCoordinateSpan(latitudeDelta: 0.65, longitudeDelta: 0.65)
        )
    )

    private var availableBoroughs: [String] {
        let boroughs = Set(store.allListings.compactMap(\.borough)).sorted()
        return ["All boroughs"] + boroughs
    }

    private var filteredListings: [HousingListing] {
        let listings = store.allListings.filter { listing in
            let priceMatches = Double(maxPrice).map { enteredMax in
                guard let monthlyRent = listing.monthlyRent else { return true }
                return Double(monthlyRent) <= enteredMax
            } ?? true
            let bedroomsMatches = listing.bedrooms >= minimumBedrooms
            let locationMatches = locationText.trimmingCharacters(in: .whitespacesAndNewlines).isEmpty ||
                listing.location.localizedCaseInsensitiveContains(locationText) ||
                listing.title.localizedCaseInsensitiveContains(locationText)
            let sourceMatches = selectedSource.map { listing.source == $0 } ?? true
            let boroughMatches = selectedBorough == "All boroughs" || listing.borough == selectedBorough
            return priceMatches && bedroomsMatches && locationMatches && sourceMatches && boroughMatches
        }

        switch sortMode {
        case .closingSoonest:
            return listings.sorted { lhs, rhs in
                switch (lhs.deadlineDate, rhs.deadlineDate) {
                case let (left?, right?): return left < right
                case (_?, nil): return true
                case (nil, _?): return false
                case (nil, nil): return lhs.title < rhs.title
                }
            }
        case .borough:
            return listings.sorted {
                ($0.borough ?? $0.location, $0.title) < ($1.borough ?? $1.location, $1.title)
            }
        case .alphabetical:
            return listings.sorted { $0.title < $1.title }
        }
    }

    var body: some View {
        List {
            Section("Listing sources") {
                ForEach(ListingSource.allCases) { source in
                    VStack(alignment: .leading, spacing: 10) {
                        HStack(spacing: 10) {
                            Image(systemName: source.systemImage)
                                .foregroundStyle(AppTheme.blue)
                                .frame(width: 34, height: 34)
                                .background(AppTheme.sky, in: RoundedRectangle(cornerRadius: 12, style: .continuous))
                            VStack(alignment: .leading, spacing: 4) {
                                Text(source.rawValue)
                                    .font(.headline)
                                    .foregroundStyle(AppTheme.ink)
                                Text(source.coverage)
                                    .font(.subheadline)
                                    .foregroundStyle(.secondary)
                            }
                            Spacer()
                            Button(selectedSource == source ? "Selected" : "Use") {
                                selectedSource = selectedSource == source ? nil : source
                            }
                            .buttonStyle(.borderedProminent)
                            .tint(selectedSource == source ? AppTheme.ink : AppTheme.blue)
                        }

                        Text(source.detail)
                            .font(.caption)
                            .foregroundStyle(.secondary)

                        Link(destination: source.destinationURL) {
                            Label("Open \(source.rawValue)", systemImage: "arrow.up.right.square")
                        }
                        .font(.subheadline.weight(.semibold))
                    }
                    .padding(.vertical, 4)
                }
            }

            Section("Housing Connect feed") {
                switch store.housingConnectFeedState {
                case .idle, .loading:
                    Label("Loading current Housing Connect lotteries...", systemImage: "arrow.triangle.2.circlepath")
                        .foregroundStyle(.secondary)
                case .loaded(let date):
                    Label("Updated \(date.formatted(date: .abbreviated, time: .shortened))", systemImage: "checkmark.circle.fill")
                        .foregroundStyle(.green)
                case .failed(let message):
                    VStack(alignment: .leading, spacing: 8) {
                        Label("Live Housing Connect feed unavailable", systemImage: "wifi.exclamationmark")
                            .foregroundStyle(.orange)
                        Text(message)
                            .font(.caption)
                            .foregroundStyle(.secondary)
                    }
                }

                Button("Refresh Housing Connect feed") {
                    Task {
                        await store.loadHousingConnectListings(force: true)
                    }
                }
            }

            Section("Native filters") {
                MoneyField(title: "Max rent", text: $maxPrice)
                Stepper("Minimum bedrooms: \(bedroomLabel(minimumBedrooms))", value: $minimumBedrooms, in: 0...4)
                TextField("Location or listing name", text: $locationText)
                    .textInputAutocapitalization(.words)
                Picker("Source", selection: $selectedSource) {
                    Text("All sources").tag(ListingSource?.none)
                    ForEach(ListingSource.allCases) { source in
                        Text(source.rawValue).tag(Optional(source))
                    }
                }
                Picker("Sort", selection: $sortMode) {
                    ForEach(ListingSortMode.allCases) { mode in
                        Text(mode.rawValue).tag(mode)
                    }
                }
            }

            Section("Boroughs") {
                ScrollView(.horizontal, showsIndicators: false) {
                    HStack(spacing: 10) {
                        ForEach(availableBoroughs, id: \.self) { borough in
                            Button(borough) {
                                selectedBorough = borough
                            }
                            .buttonStyle(.borderedProminent)
                            .tint(selectedBorough == borough ? AppTheme.ink : AppTheme.blue.opacity(0.2))
                            .foregroundStyle(selectedBorough == borough ? .white : AppTheme.ink)
                        }
                    }
                    .padding(.vertical, 2)
                }
                Button("Clear filters") {
                    maxPrice = ""
                    minimumBedrooms = 0
                    locationText = ""
                    selectedSource = nil
                    selectedBorough = "All boroughs"
                    sortMode = .closingSoonest
                }
                .disabled(maxPrice.isEmpty && minimumBedrooms == 0 && locationText.isEmpty && selectedSource == nil && selectedBorough == "All boroughs" && sortMode == .closingSoonest)
            }

            Section("Map") {
                Map(position: $mapPosition) {
                    ForEach(filteredListings) { listing in
                        Marker(listing.title, coordinate: listing.coordinate)
                    }
                }
                .frame(height: 220)
                .clipShape(RoundedRectangle(cornerRadius: 18, style: .continuous))
                Text("Map pins combine live Housing Connect lotteries with your native saved-and-compare samples.")
                    .font(.caption)
                    .foregroundStyle(.secondary)
            }

            Section("\(filteredListings.count) listings") {
                if filteredListings.isEmpty {
                    EmptyStateView(symbol: "line.3.horizontal.decrease.circle", title: "No matching listings", message: "Adjust price, bedroom, or location filters to see native sample listings.")
                } else {
                    ForEach(filteredListings) { listing in
                        NavigationLink {
                            ListingDetailView(listing: listing)
                        } label: {
                            ListingRow(listing: listing)
                        }
                    }
                }
            }
        }
        .navigationTitle("Listings")
        .task {
            await store.loadHousingConnectListings()
        }
    }
}

struct ListingDetailView: View {
    @EnvironmentObject private var store: HousingNavStore
    let listing: HousingListing
    @State private var draftNote = ""
    @State private var confirmation: String?

    var body: some View {
        List {
            Section {
                VStack(alignment: .leading, spacing: 10) {
                    Text(listing.title)
                        .font(.title2.weight(.bold))
                        .foregroundStyle(AppTheme.ink)
                    Label(listing.source.rawValue, systemImage: listing.source.systemImage)
                        .font(.caption.weight(.semibold))
                        .foregroundStyle(AppTheme.blue)
                    if let status = listing.statusText {
                        Label(status, systemImage: "flag.fill")
                            .font(.caption)
                            .foregroundStyle(.secondary)
                    }
                    if let urgency = listing.urgencyText {
                        Label(urgency, systemImage: "clock.badge.exclamationmark")
                            .font(.caption.weight(.semibold))
                            .foregroundStyle(urgency == "Closing soon" || urgency == "Closes today" ? .orange : .secondary)
                    }
                    if let deadline = listing.deadlineText {
                        Label("Deadline \(deadline)", systemImage: "calendar")
                            .font(.caption)
                            .foregroundStyle(.secondary)
                    }
                    Text(listing.summary)
                        .foregroundStyle(.secondary)
                    HStack {
                        Label(listingRentText(listing), systemImage: "dollarsign.circle")
                        Spacer()
                        Label(listing.bedroomSummary, systemImage: "bed.double")
                    }
                    .font(.subheadline.weight(.semibold))
                }
                .padding(.vertical, 6)
            }

            Section("Source") {
                Text(listing.source.coverage)
                    .foregroundStyle(.secondary)
                if let listingURL = listing.listingURL {
                    Link(destination: listingURL) {
                        Label("Open \(listing.source.rawValue)", systemImage: "arrow.up.right.square")
                    }
                }
            }

            if listing.source == .housingConnect {
                Section("Housing Connect details") {
                    if let developmentType = listing.developmentType {
                        LabeledContent("Type", value: developmentType)
                    }
                    if let unitCount = listing.unitCount {
                        LabeledContent("Units", value: "\(unitCount)")
                    }
                    if !listing.unitMixDetails.isEmpty {
                        VStack(alignment: .leading, spacing: 8) {
                            Text("Unit mix")
                                .font(.subheadline.weight(.semibold))
                            ForEach(listing.unitMixDetails, id: \.self) { mix in
                                Label(mix, systemImage: "square.grid.2x2")
                                    .font(.caption)
                                    .foregroundStyle(.secondary)
                            }
                        }
                    }
                    if let amiSummary = listing.amiSummary {
                        VStack(alignment: .leading, spacing: 8) {
                            Text("AMI breakdown")
                                .font(.subheadline.weight(.semibold))
                            Text(amiSummary)
                                .font(.caption)
                                .foregroundStyle(.secondary)
                        }
                    }
                }
            }

            Section("Saved listing") {
                Button {
                    store.toggleFavoriteListing(listing)
                    confirmation = store.isFavoriteListing(listing) ? "Saved to favorites" : "Removed from favorites"
                } label: {
                    Label(store.isFavoriteListing(listing) ? "Remove from favorites" : "Save listing", systemImage: store.isFavoriteListing(listing) ? "heart.fill" : "heart")
                }
            }

            Section("Private notes") {
                TextEditor(text: $draftNote)
                    .frame(minHeight: 120)
                Button("Save note") {
                    store.updateNote(draftNote, for: listing)
                    confirmation = "Note saved locally"
                }
                .buttonStyle(.borderedProminent)
            }

            Section("Native map") {
                Map {
                    Marker(listing.title, coordinate: listing.coordinate)
                }
                .frame(height: 200)
                .clipShape(RoundedRectangle(cornerRadius: 18, style: .continuous))
            }
        }
        .navigationTitle("Listing")
        .navigationBarTitleDisplayMode(.inline)
        .onAppear {
            draftNote = store.note(for: listing)
        }
        .safeAreaInset(edge: .bottom) {
            if let confirmation {
                Label(confirmation, systemImage: "checkmark.circle.fill")
                    .font(.caption.weight(.semibold))
                    .foregroundStyle(.green)
                    .padding(.horizontal, 14)
                    .padding(.vertical, 10)
                    .background(.regularMaterial, in: Capsule())
                    .padding(.bottom, 8)
            }
        }
    }
}

struct CompareListingsView: View {
    @EnvironmentObject private var store: HousingNavStore

    var body: some View {
        List {
            if store.favoriteListings.isEmpty {
                EmptyStateView(symbol: "heart", title: "No saved listings yet", message: "Save listings from the Listings tab to compare rent, bedrooms, location, and notes here.")
            } else {
                Section("Saved comparison") {
                    ForEach(store.favoriteListings) { listing in
                        VStack(alignment: .leading, spacing: 10) {
                            Text(listing.title)
                                .font(.headline)
                                .foregroundStyle(AppTheme.ink)
                            HStack {
                                ComparisonPill(title: "Rent", value: listingRentText(listing))
                                ComparisonPill(title: "Beds", value: listing.bedroomSummary)
                                ComparisonPill(title: "Area", value: listing.location)
                            }
                            ComparisonPill(title: "Source", value: listing.source.rawValue)
                            if let deadline = listing.deadlineText {
                                ComparisonPill(title: "Deadline", value: deadline)
                            }
                            if !store.note(for: listing).isEmpty {
                                Label(store.note(for: listing), systemImage: "note.text")
                                    .font(.caption)
                                    .foregroundStyle(.secondary)
                            }
                        }
                        .padding(.vertical, 6)
                    }
                }
            }
        }
        .navigationTitle("Compare")
    }
}

struct SupportView: View {
    private var appVersion: String {
        let version = Bundle.main.infoDictionary?["CFBundleShortVersionString"] as? String ?? "1.0"
        let build = Bundle.main.infoDictionary?["CFBundleVersion"] as? String ?? "1"
        return "\(version) (\(build))"
    }

    var body: some View {
        List {
            Section("HousingNav Support") {
                CompactCard(symbol: "envelope", title: "Contact support", subtitle: "support@housingnav.example")
                Link(destination: URL(string: "mailto:support@housingnav.example?subject=HousingNav%20Support")!) {
                    Label("Email support", systemImage: "paperplane.fill")
                }
                Text("Replace the placeholder email with your real support inbox before release.")
                    .font(.caption)
                    .foregroundStyle(.secondary)
            }

            Section("FAQ") {
                DisclosureGroup("Does HousingNav require an account?") {
                    Text("No. The native calculator, saved listings, notes, checklists, and compare tools work locally on device.")
                }
                DisclosureGroup("Where are saved listings and notes stored?") {
                    Text("Favorites and notes are stored locally on the device using app storage.")
                }
                DisclosureGroup("Are calculator results final approvals?") {
                    Text("No. HousingNav provides planning estimates. Final eligibility, rent approval, and lease-up decisions are made by the administering agency.")
                }
            }

            Section("App") {
                LabeledContent("Version", value: appVersion)
                LabeledContent("Device name", value: "HousingNav")
            }
        }
        .navigationTitle("Help & Support")
    }
}

struct VoucherHelpView: View {
    var body: some View {
        List {
            Section("Native quick actions") {
                CompactCard(symbol: "doc.on.doc", title: "Collect documents", subtitle: "Photo ID, income documents, voucher paperwork, and appointment notes.")
                CompactCard(symbol: "calendar.badge.clock", title: "Track deadlines", subtitle: "Use saved plans and checklists to keep next steps visible.")
                CompactCard(symbol: "wrench.and.screwdriver", title: "Prepare for inspection", subtitle: "Confirm utilities, repairs, access, and safety basics before the visit.")
            }

            Section("Next") {
                NavigationLink("Open applicant calculator") {
                    ApplicantCalculatorView()
                }

                NavigationLink("Ask Betty") {
                    BettyChatView(role: .applicant)
                }
            }
        }
        .navigationTitle("Voucher Help")
    }
}

struct ResourcesView: View {
    let role: AppRole

    var body: some View {
        List {
            Section("Native housing guide") {
                Text(role == .applicant ? "Applicant guidance is saved to this route, alongside tenant calculators, Housing Connect prep, saved plans, Betty, and native sharing." : "Landlord guidance is saved to this route, alongside packet prep, landlord estimates, Betty, checklists, and native sharing.")
            }

            if role == .applicant {
                Section("Housing Connect help") {
                    ResourceGroup(role: role, "Voucher basics", steps: [
                        "Section 8 households usually pay about 30% of adjusted income, while the housing authority pays the balance up to the applicable payment standard.",
                        "CityFHEPS is administered by DSS and helps eligible households find and keep housing across New York State.",
                        "FHEPS is for eligible Cash Assistance households with children, including certain families facing eviction, domestic violence, or serious housing-condition issues.",
                        "Voucher households should not rely only on a normal income-band calculator. Shopping letters, subsidy rules, and agency review matter."
                    ])

                    ResourceGroup(role: role, "Augmented CityFHEPS", steps: [
                        "DHS shelter clients may be eligible for Augmented CityFHEPS for units advertised on Housing Connect.",
                        "A shelter letter may be enough proof for Augmented CityFHEPS eligibility for DHS shelter clients.",
                        "If someone is in a DHS shelter, they should check the ad and confirm shopping-letter or shelter documentation rules."
                    ])

                    ResourceGroup(role: role, "Section 8 exception standards", steps: [
                        "Some ZIP codes use higher exception payment standards to support moves to higher-opportunity areas.",
                        "NYCHA and HPD standards can differ, so ZIP, bedroom size, and administering agency matter.",
                        "HousingNav includes supported NYCHA ZIP exception standards in the native calculator, but final agency charts should control.",
                        "Standard minimum-income rules may not apply the same way to tenant-based Section 8 and other qualifying rental subsidies."
                    ])

                    ResourceGroup(role: role, "Profile and documents", steps: [
                        "Create one complete household profile with every person who will live in the apartment.",
                        "Keep email, phone, and mailing address current because document requests can expire quickly.",
                        "Match legal names and addresses across IDs, tax returns, pay stubs, vouchers, benefit letters, and household records.",
                        "Common uploads include ID, birth records, income documents, asset records, current housing proof, preference proof, and voucher papers."
                    ])

                    ResourceGroup(role: role, "Gross income tips", steps: [
                        "Use household income for all people who will live in the apartment when the application asks for it.",
                        "Housing ads usually show annual income ranges, so convert wages and benefits into yearly amounts.",
                        "Benefits, child support, Social Security, pensions, and public benefits may matter depending on listing rules.",
                        "Watch both the minimum and maximum income limits."
                    ])
                }

                Section("Lottery guide") {
                    ResourceGroup(role: role, "Lottery steps", steps: [
                        "Apply before the deadline. Online and mailed applications can have the same chance if submitted on time.",
                        "After the deadline, applications are randomized and assigned log numbers.",
                        "Preference and set-aside batches are usually reviewed before the general pool.",
                        "Eligibility review may ask for documents confirming household size, income, address, and other details.",
                        "Agency review may follow before a lease can move forward.",
                        "The outcome may be an offer, no unit available, or rejection with appeal instructions."
                    ])

                    ResourceGroup(role: role, "Set-asides and preferences", steps: [
                        "Disability set-asides may include mobility, hearing, and vision categories in qualifying projects.",
                        "Community preference may reserve a project-specific share of units for residents of the building's Community District.",
                        "Some lotteries include preference for eligible NYC municipal employees and military veterans.",
                        "NYC residents in the general pool are generally processed before non-residents after applicable preference batches.",
                        "Homeless referral or other agency-approved categories may apply depending on the project.",
                        "Always read the ad because preferences are not identical across every listing."
                    ])

                    ResourceGroup(role: role, "Call order and timing", steps: [
                        "Applications are not usually reviewed in the order people clicked submit.",
                        "A lower log number can help, but unit size, income band, preferences, set-asides, and documents also matter.",
                        "Being contacted usually means document or eligibility review, not a guaranteed lease.",
                        "Processing can take months after the deadline, and some applicants may never hear back on a specific lottery.",
                        "Reply quickly if contacted because document deadlines may be short."
                    ])

                    ResourceGroup(role: role, "Fast applicant prep", steps: [
                        "Save clean PDFs of pay stubs, IDs, benefit letters, and voucher documents now.",
                        "Check unit size, income range, preference boxes, and deadlines line by line.",
                        "Keep confirmation numbers, screenshots, and messages organized."
                    ])
                }
            }

            if role == .landlord {
                Section("Landlord resources") {
                ResourceGroup(role: role, "Common landlord packet checklist", steps: [
                    "Completed landlord or owner packet form for the specific program.",
                    "Current W-9 for tax identification and payment setup.",
                    "Signed lease template or sample lease with voucher clauses if required.",
                    "Proof of property ownership, deed, or management agreement.",
                    "Lead paint disclosure for pre-1978 properties when applicable.",
                    "Tenant application, screening criteria, unit description, photos, ACH/payment setup, and owner contact information."
                ])

                ResourceGroup(role: role, "Submitting a packet", steps: [
                    "Collect required documents and scan them as clean PDFs.",
                    "Complete program-specific landlord forms from the administering agency.",
                    "Confirm the submission channel: portal upload, email, or mail.",
                    "Use a clear subject line with tenant name, property address, and voucher number if available.",
                    "Keep copies of everything and request confirmation or a case number.",
                    "Prepare the unit for inspection before the packet gets deep into review."
                ])

                ResourceGroup(role: role, "Program quick guides", steps: [
                    "CityFHEPS and FHEPS commonly involve landlord packet forms, W-9, ACH enrollment, lease sample, ownership proof, unit description, and photos.",
                    "HPD Section 8 may require landlord enrollment, W-9, vendor payment form, signed lease, tenant application, unit description, photos, and lead-based paint disclosures.",
                    "NYCHA Section 8 enrollment is handled online and is tied to the case process, packet review, rent reasonableness, and inspection.",
                    "HOME TBRA is a separate HPD rental-assistance path and should not be assumed to be identical to EHV."
                ])

                ResourceGroup(role: role, "HPD packet email format", steps: [
                    "For HPD Section 8 submissions, use a clear packet email with tenant name, property address, voucher number if known, owner name, and phone number.",
                    "Attach the completed packet, lease, ownership/payment documents, and any preliminary lease materials requested.",
                    "Ask for receipt confirmation and keep the sent email with attachments for your records."
                ])

                ResourceGroup(role: role, "EHV and HOME TBRA note", steps: [
                    "Emergency Housing Voucher funding is a time-sensitive transition issue in 2026.",
                    "HPD may invite current participating EHV households to apply for HOME Tenant-Based Rental Assistance.",
                    "NYCHA may identify alternative subsidized housing options and conduct outreach to impacted EHV participants.",
                    "Landlords should verify current agency guidance before assuming an EHV subsidy path remains unchanged."
                ])
                }

                Section("Landlord FAQ") {
                ResourceGroup(role: role, "Do Section 8, CityFHEPS, and FHEPS work the same way?", steps: [
                    "No. They all involve forms, lease paperwork, and payment setup, but lease-up paths differ.",
                    "NYCHA Section 8, HPD Section 8, CityFHEPS, and FHEPS each have different review and submission workflows.",
                    "CityFHEPS and FHEPS often depend on tenant-side case records, shopping letters, caseworker follow-up, and apartment review."
                ])

                ResourceGroup(role: role, "How should landlords think about Section 8 lease-up?", steps: [
                    "Lease-up depends on a complete packet, rent and utility review, and an inspection-ready unit.",
                    "NYCHA owners should expect rent reasonableness review, packet review, and an initial or transfer inspection before move-in.",
                    "If packet, rent, utility, or inspection items are off, the process can slow down."
                ])

                ResourceGroup(role: role, "What should landlords know about inspections?", steps: [
                    "Inspections are one of the biggest reasons lease-up timing becomes unpredictable.",
                    "Smoke alarms, carbon monoxide compliance, outlets, plumbing, heat, windows, and general condition are move-in-critical.",
                    "If repairs are needed, the timeline usually stretches because the file cannot move like a pass-on-first-inspection case."
                ])

                ResourceGroup(role: role, "How fast is lease-up?", steps: [
                    "Lease-up speed is not predictable enough to promise in days.",
                    "Timing depends on packet completeness, caseworker response, inspection availability, rent review, tenant documents, ownership documents, and repairs.",
                    "Avoid promising a firm move-in date until agency review and inspection are clearly in motion."
                ])

                ResourceGroup(role: role, "CityFHEPS and FHEPS follow-up", steps: [
                    "CityFHEPS cases may require follow-up through shelter staff, a caseworker, Homebase, or DSS-connected support.",
                    "FHEPS may require follow-up with a caseworker, Benefits Access Center, or Homebase-linked path.",
                    "Silence does not always mean the case is dead; tenant-side paperwork or approvals may still need movement."
                ])

                ResourceGroup(role: role, "Tracking packet submissions", steps: [
                    "Use a clean PDF set, clear subject lines, copies of everything sent, and confirmation requests.",
                    "Track whether delays are on the landlord packet, tenant file, worker follow-up, inspection, or rent review side.",
                    "Use payment standard and utility allowance as estimate inputs only.",
                    "Final approval can still change after agency review, rent reasonableness, inspection, or mixed-family rules."
                ])
                }
            }
        }
        .navigationTitle(role == .applicant ? "Applicant Guides" : "Landlord Guides")
    }
}

struct ResourceGroup: View {
    @EnvironmentObject private var store: HousingNavStore
    let role: AppRole
    let title: String
    let steps: [String]

    private var completedCount: Int {
        store.completedGuideSteps(role: role, guideTitle: title, steps: steps)
    }

    private var nextStep: String {
        steps.first { !store.isGuideStepDone(role: role, guideTitle: title, step: $0) } ?? "All steps complete. Review your checklist or save the result to your plan."
    }

    init(role: AppRole, _ title: String, steps: [String]) {
        self.role = role
        self.title = title
        self.steps = steps
    }

    var body: some View {
        DisclosureGroup(title) {
            VStack(alignment: .leading, spacing: 12) {
                ProgressView(value: Double(completedCount), total: Double(max(steps.count, 1)))
                Text("\(completedCount)/\(steps.count) steps completed")
                    .font(.caption.weight(.semibold))
                    .foregroundStyle(.secondary)

                ForEach(steps, id: \.self) { step in
                    ResourceStep(
                        step,
                        isDone: store.isGuideStepDone(role: role, guideTitle: title, step: step)
                    ) {
                        store.toggleGuideStep(role: role, guideTitle: title, step: step)
                    }
                }

                VStack(alignment: .leading, spacing: 8) {
                    Label("Next step", systemImage: "arrow.forward.circle.fill")
                        .font(.caption.weight(.bold))
                        .foregroundStyle(AppTheme.blue)
                    Text(nextStep)
                        .font(.subheadline)
                        .foregroundStyle(AppTheme.ink)

                    HStack {
                        Button {
                            store.addChecklistItem(role: role, title: title, detail: nextStep)
                        } label: {
                            Label("Add to Checklist", systemImage: "checklist")
                        }
                        .buttonStyle(.bordered)

                        Button {
                            store.addChecklistItem(role: role, title: "My Plan: \(title)", detail: nextStep)
                        } label: {
                            Label("Save to My Plan", systemImage: "tray.and.arrow.down")
                        }
                        .buttonStyle(.borderedProminent)
                    }
                    .font(.caption)
                }
                .padding(12)
                .background(AppTheme.sky.opacity(0.65), in: RoundedRectangle(cornerRadius: 16, style: .continuous))
            }
        }
    }
}

struct ResourceStep: View {
    let text: String
    let isDone: Bool
    let onToggle: () -> Void

    private var actionTitle: String {
        guard let periodIndex = text.firstIndex(of: ".") else { return text }
        return String(text[..<periodIndex])
    }

    private var detailText: String? {
        guard let periodIndex = text.firstIndex(of: ".") else { return nil }
        let detailStart = text.index(after: periodIndex)
        let detail = text[detailStart...].trimmingCharacters(in: .whitespacesAndNewlines)
        return detail.isEmpty ? nil : detail
    }

    init(_ text: String, isDone: Bool = false, onToggle: @escaping () -> Void = {}) {
        self.text = text
        self.isDone = isDone
        self.onToggle = onToggle
    }

    var body: some View {
        Button(action: onToggle) {
            HStack(alignment: .top, spacing: 10) {
                Image(systemName: isDone ? "checkmark.circle.fill" : "circle")
                    .foregroundStyle(isDone ? .green : .secondary)
                VStack(alignment: .leading, spacing: 3) {
                    Text(actionTitle)
                        .font(.subheadline.weight(.semibold))
                        .foregroundStyle(isDone ? .secondary : AppTheme.ink)
                        .strikethrough(isDone)
                    if let detailText {
                        Text(detailText)
                            .font(.caption)
                            .foregroundStyle(.secondary)
                    }
                }
                Spacer(minLength: 0)
            }
            .padding(.vertical, 4)
            .contentShape(Rectangle())
        }
        .buttonStyle(.plain)
    }
}

struct DashboardShell<Content: View>: View {
    let title: String
    let subtitle: String
    @ViewBuilder var content: Content

    var body: some View {
        ZStack {
            AppBackground()

            ScrollView {
                VStack(alignment: .leading, spacing: 18) {
                    VStack(alignment: .leading, spacing: 8) {
                        Text(title)
                            .font(.system(size: 34, weight: .heavy, design: .rounded))
                            .foregroundStyle(AppTheme.ink)
                        Text(subtitle)
                            .font(.subheadline)
                            .foregroundStyle(.secondary)
                    }

                    content
                }
                .padding(20)
            }
        }
    }
}

struct RoleSwitcherCard: View {
    let currentRole: AppRole
    let onSwitchRole: () -> Void
    let onChooseRole: () -> Void

    private var destinationLabel: String {
        currentRole == .applicant ? "Switch to Landlord" : "Switch to Applicant / Tenant"
    }

    private var currentRouteLabel: String {
        currentRole == .applicant ? "Applicant / Tenant route active" : "Landlord route active"
    }

    var body: some View {
        VStack(alignment: .leading, spacing: 12) {
            HStack {
                Label(currentRouteLabel, systemImage: "point.topleft.down.curvedto.point.bottomright.up")
                    .font(.subheadline.weight(.semibold))
                    .foregroundStyle(AppTheme.ink)
                Spacer()
            }

            Text("Switch roles at any time without losing saved plans, estimates, answers, or checklist progress.")
                .font(.caption)
                .foregroundStyle(.secondary)

            HStack {
                Button(destinationLabel, action: onSwitchRole)
                    .buttonStyle(.borderedProminent)

                Button("Choose Route", action: onChooseRole)
                    .buttonStyle(.bordered)
            }
        }
        .padding(16)
        .background(AppTheme.card, in: RoundedRectangle(cornerRadius: 24, style: .continuous))
        .shadow(color: .black.opacity(0.05), radius: 10, y: 5)
    }
}

struct DashboardGrid<Content: View>: View {
    @ViewBuilder var content: Content

    var body: some View {
        LazyVGrid(columns: [GridItem(.flexible()), GridItem(.flexible())], spacing: 14) {
            content
        }
        .buttonStyle(.plain)
    }
}

struct DashboardCard: View {
    let symbol: String
    let title: String
    let subtitle: String

    var body: some View {
        VStack(alignment: .leading, spacing: 12) {
            Image(systemName: symbol)
                .font(.title2.weight(.bold))
                .foregroundStyle(AppTheme.blue)
                .frame(width: 42, height: 42)
                .background(AppTheme.sky, in: RoundedRectangle(cornerRadius: 14, style: .continuous))

            Text(title)
                .font(.headline)
                .foregroundStyle(AppTheme.ink)

            Text(subtitle)
                .font(.caption)
                .foregroundStyle(.secondary)
                .lineLimit(3)

            Spacer(minLength: 0)
        }
        .frame(maxWidth: .infinity, minHeight: 158, alignment: .topLeading)
        .padding(16)
        .background(AppTheme.card, in: RoundedRectangle(cornerRadius: 24, style: .continuous))
        .shadow(color: .black.opacity(0.06), radius: 14, y: 8)
    }
}

struct RoleCard: View {
    let role: AppRole
    let title: String
    let subtitle: String

    var body: some View {
        HStack(spacing: 16) {
            Image(systemName: role.symbol)
                .font(.title.weight(.bold))
                .foregroundStyle(.white)
                .frame(width: 64, height: 64)
                .background(AppTheme.blue, in: RoundedRectangle(cornerRadius: 22, style: .continuous))

            VStack(alignment: .leading, spacing: 5) {
                Text(title)
                    .font(.title3.weight(.bold))
                    .foregroundStyle(AppTheme.ink)
                Text(subtitle)
                    .font(.subheadline)
                    .foregroundStyle(.secondary)
            }

            Spacer()
            Image(systemName: "chevron.right")
                .font(.headline)
                .foregroundStyle(.secondary)
        }
        .padding(18)
        .frame(maxWidth: .infinity, alignment: .leading)
        .background(AppTheme.card, in: RoundedRectangle(cornerRadius: 28, style: .continuous))
        .contentShape(RoundedRectangle(cornerRadius: 28, style: .continuous))
        .shadow(color: .black.opacity(0.07), radius: 18, y: 9)
    }
}

struct RoleExplainerCard: View {
    let title: String
    let subtitle: String

    var body: some View {
        VStack(alignment: .leading, spacing: 6) {
            Text(title)
                .font(.headline)
                .foregroundStyle(AppTheme.ink)
            Text(subtitle)
                .font(.subheadline)
                .foregroundStyle(.secondary)
        }
        .frame(maxWidth: .infinity, alignment: .leading)
        .padding(18)
        .background(.white.opacity(0.75), in: RoundedRectangle(cornerRadius: 24, style: .continuous))
    }
}

struct ProgressHero: View {
    let title: String
    let value: Int
    let total: Int

    var progress: Double {
        total > 0 ? Double(value) / Double(total) : 0
    }

    var body: some View {
        VStack(alignment: .leading, spacing: 12) {
            HStack {
                Text(title)
                    .font(.headline)
                Spacer()
                Text("\(value)/\(total)")
                    .font(.headline.weight(.bold))
                    .foregroundStyle(AppTheme.blue)
            }

            ProgressView(value: progress)
                .tint(AppTheme.blue)

            Text("Progress is saved locally and restored the next time you open HousingNav.")
                .font(.caption)
                .foregroundStyle(.secondary)
        }
        .padding(18)
        .background(AppTheme.sand, in: RoundedRectangle(cornerRadius: 24, style: .continuous))
    }
}

struct StarterPromptStrip: View {
    let role: AppRole
    let onPick: (String) -> Void

    private var prompts: [String] {
        switch role {
        case .applicant:
            [
                "What should I prepare before a voucher inspection?",
                "How do I organize Housing Connect documents?",
                "What does my calculator result mean?"
            ]
        case .landlord:
            [
                "What usually goes in a landlord packet?",
                "How should I prepare for inspection?",
                "How do I explain my rent estimate?"
            ]
        }
    }

    var body: some View {
        ScrollView(.horizontal, showsIndicators: false) {
            HStack {
                ForEach(prompts, id: \.self) { prompt in
                    Button(prompt) {
                        onPick(prompt)
                    }
                    .font(.caption.weight(.semibold))
                    .padding(.horizontal, 14)
                    .padding(.vertical, 10)
                    .background(AppTheme.sky, in: Capsule())
                }
            }
        }
    }
}

struct ChatBubble: View {
    @EnvironmentObject private var store: HousingNavStore
    let message: ChatMessage
    let role: AppRole
    @State private var confirmation: String?

    var body: some View {
        HStack {
            if message.sender == .user {
                Spacer(minLength: 36)
            }

            VStack(alignment: .leading, spacing: 10) {
                Text(message.text)
                    .font(.body)

                if message.sender == .betty {
                    HStack {
                        Button {
                            store.saveBettyAnswer(message.text, role: role)
                            confirmation = "Saved answer"
                        } label: {
                            Label("Save answer", systemImage: "bookmark")
                        }

                        Button {
                            store.addChecklistItem(role: role, title: "Betty: \(HousingNavStore.inferredTagName(for: message.text, role: role))", detail: message.text)
                            confirmation = "Added to checklist"
                        } label: {
                            Label("Add to checklist", systemImage: "checklist")
                        }
                    }
                    .font(.caption)
                    .buttonStyle(.bordered)

                    if let confirmation {
                        Label(confirmation, systemImage: "checkmark.circle.fill")
                            .font(.caption2.weight(.semibold))
                            .foregroundStyle(.green)
                    }
                }
            }
            .padding(14)
            .background(message.sender == .user ? AppTheme.blue : Color.white, in: RoundedRectangle(cornerRadius: 18, style: .continuous))
            .foregroundStyle(message.sender == .user ? .white : AppTheme.ink)

            if message.sender == .betty {
                Spacer(minLength: 36)
            }
        }
    }
}

struct ResultCard: View {
    let title: String
    let rows: [(String, String)]
    let shareText: String

    var body: some View {
        VStack(alignment: .leading, spacing: 14) {
            Text(title)
                .font(.headline)
                .foregroundStyle(AppTheme.ink)

            ForEach(rows, id: \.0) { row in
                HStack {
                    Text(row.0)
                        .foregroundStyle(.secondary)
                    Spacer()
                    Text(row.1)
                        .fontWeight(.bold)
                }
            }

            ShareLink(item: shareText) {
                Label("Share summary", systemImage: "square.and.arrow.up")
                    .frame(maxWidth: .infinity)
            }
            .buttonStyle(.borderedProminent)
        }
        .padding(.vertical, 8)
    }
}

struct SavedResultRow: View {
    let title: String
    let subtitle: String
    let createdAt: Date

    var body: some View {
        VStack(alignment: .leading, spacing: 5) {
            Text(title)
                .font(.headline)
            Text(subtitle)
                .font(.subheadline)
                .foregroundStyle(.secondary)
            Text(createdAt.formatted(date: .abbreviated, time: .shortened))
                .font(.caption)
                .foregroundStyle(.tertiary)
        }
        .padding(.vertical, 4)
    }
}

struct ListingRow: View {
    @EnvironmentObject private var store: HousingNavStore
    let listing: HousingListing

    var body: some View {
        HStack(alignment: .top, spacing: 12) {
            Image(systemName: store.isFavoriteListing(listing) ? "heart.fill" : "building.2")
                .foregroundStyle(store.isFavoriteListing(listing) ? .red : AppTheme.blue)
                .frame(width: 34, height: 34)
                .background(AppTheme.sky, in: RoundedRectangle(cornerRadius: 12, style: .continuous))

            VStack(alignment: .leading, spacing: 5) {
                Text(listing.title)
                    .font(.headline)
                    .foregroundStyle(AppTheme.ink)
                Text("\(listing.location) • \(listing.bedroomSummary) • \(listingRentText(listing))")
                    .font(.subheadline)
                    .foregroundStyle(.secondary)
                Label(listing.source.rawValue, systemImage: listing.source.systemImage)
                    .font(.caption.weight(.semibold))
                    .foregroundStyle(AppTheme.blue)
                if let urgency = listing.urgencyText {
                    Label(urgency, systemImage: "clock.badge.exclamationmark")
                        .font(.caption.weight(.semibold))
                        .foregroundStyle(urgency == "Closing soon" || urgency == "Closes today" ? .orange : .secondary)
                }
                if let deadline = listing.deadlineText {
                    Label("Deadline \(deadline)", systemImage: "calendar")
                        .font(.caption)
                        .foregroundStyle(.secondary)
                }
                if !store.note(for: listing).isEmpty {
                    Label("Note saved", systemImage: "note.text")
                        .font(.caption.weight(.semibold))
                        .foregroundStyle(AppTheme.blue)
                }
            }
        }
        .padding(.vertical, 4)
    }
}

struct ComparisonPill: View {
    let title: String
    let value: String

    var body: some View {
        VStack(alignment: .leading, spacing: 3) {
            Text(title)
                .font(.caption2.weight(.bold))
                .foregroundStyle(.secondary)
            Text(value)
                .font(.caption.weight(.semibold))
                .foregroundStyle(AppTheme.ink)
                .lineLimit(1)
        }
        .frame(maxWidth: .infinity, alignment: .leading)
        .padding(10)
        .background(AppTheme.sky.opacity(0.7), in: RoundedRectangle(cornerRadius: 14, style: .continuous))
    }
}

struct MoneyField: View {
    let title: String
    @Binding var text: String

    var body: some View {
        TextField(title, text: $text)
            .keyboardType(.decimalPad)
    }
}

struct CompactCard: View {
    let symbol: String
    let title: String
    let subtitle: String

    var body: some View {
        HStack(spacing: 12) {
            Image(systemName: symbol)
                .foregroundStyle(AppTheme.blue)
                .frame(width: 32)

            VStack(alignment: .leading, spacing: 3) {
                Text(title)
                    .font(.headline)
                    .foregroundStyle(AppTheme.ink)
                Text(subtitle)
                    .font(.caption)
                    .foregroundStyle(.secondary)
            }
        }
        .frame(maxWidth: .infinity, alignment: .leading)
        .padding(14)
        .background(Color.white.opacity(0.8), in: RoundedRectangle(cornerRadius: 18, style: .continuous))
    }
}

struct EmptyStateView: View {
    let symbol: String
    let title: String
    let message: String

    var body: some View {
        VStack(spacing: 12) {
            Image(systemName: symbol)
                .font(.largeTitle)
                .foregroundStyle(AppTheme.blue)
            Text(title)
                .font(.headline)
            Text(message)
                .font(.subheadline)
                .foregroundStyle(.secondary)
                .multilineTextAlignment(.center)
        }
        .padding(28)
    }
}

struct AppBackground: View {
    var body: some View {
        LinearGradient(
            colors: [AppTheme.sky, Color.white, AppTheme.sand.opacity(0.65)],
            startPoint: .topLeading,
            endPoint: .bottomTrailing
        )
        .ignoresSafeArea()
        .allowsHitTesting(false)
        .overlay(alignment: .topTrailing) {
            Circle()
                .fill(AppTheme.mint.opacity(0.5))
                .frame(width: 220)
                .blur(radius: 20)
                .offset(x: 90, y: -80)
        }
    }
}

struct PrimaryButtonStyle: ButtonStyle {
    func makeBody(configuration: Configuration) -> some View {
        configuration.label
            .font(.headline.weight(.bold))
            .foregroundStyle(.white)
            .padding(.vertical, 16)
            .background(AppTheme.ink.opacity(configuration.isPressed ? 0.8 : 1), in: RoundedRectangle(cornerRadius: 20, style: .continuous))
    }
}

func formatCurrency(_ value: Double) -> String {
    value.formatted(.currency(code: Locale.current.currency?.identifier ?? "USD"))
}

func listingRentText(_ listing: HousingListing) -> String {
    listing.monthlyRent.map { formatCurrency(Double($0)) } ?? "Rent not listed"
}

func bedroomLabel(_ bedrooms: Int) -> String {
    bedrooms == 0 ? "Studio" : "\(bedrooms) BR"
}
