package rdno_core

import (
	denv "github.com/jurgen-kluft/ccode/denv"
	ccore "github.com/jurgen-kluft/ccore/package"
	cunittest "github.com/jurgen-kluft/cunittest/package"
)

// Core package for Arduino projects, both for ESP32 and ESP8266.
// This package depends on the fundamental ccore package.
//
// It can compile for Desktop (Windows, Mac, Linux) but it mocks many of the
// systems that are available on the Arduino platform.
// For example:
// - GPIO
// - Serial
// - Wire
// - I2C
// - UART
// - WiFi

const (
	repo_path = "github.com\\jurgen-kluft"
	repo_name = "rdno_core"
)

func GetPackage() *denv.Package {
	name := repo_name

	// dependencies
	ccorepkg := ccore.GetPackage()
	cunittestpkg := cunittest.GetPackage()

	// main package
	mainpkg := denv.NewPackage(repo_path, repo_name)
	mainpkg.AddPackage(ccorepkg)
	mainpkg.AddPackage(cunittestpkg)

	// esp32 core library
	esp32corelib := denv.SetupCppLibProjectForArduinoEsp32(mainpkg, name+"-esp32")
	esp32corelib.AddDependencies(ccorepkg.GetMainLib())
	esp32corelib.AddEnvironmentVariable("ESP32_SDK")
	esp32corelib.AddInclude("{ESP32_SDK}", "cores/esp32", "")
	esp32corelib.AddInclude("{ESP32_SDK}", "libraries/Wire", "src")
	esp32corelib.AddInclude("{ESP32_SDK}", "libraries/SPI", "src")
	esp32corelib.AddInclude("{ESP32_SDK}", "libraries/EEPROM", "src")
	esp32corelib.SourceFilesFrom("{ESP32_SDK}", "cores/esp32", "", ".c", ".cpp", ".S")
	esp32corelib.SourceFilesFrom("{ESP32_SDK}", "libraries/Wire", "src", ".c", ".cpp")
	esp32corelib.SourceFilesFrom("{ESP32_SDK}", "libraries/SPI", "src", ".c", ".cpp")
	esp32corelib.SourceFilesFrom("{ESP32_SDK}", "libraries/EEPROM", "src", ".c", ".cpp")

	// esp8266 core library
	esp8266corelib := denv.SetupCppLibProjectForArduinoEsp8266(mainpkg, name+"-esp8266")
	esp8266corelib.AddDependencies(ccorepkg.GetMainLib())
	esp8266corelib.AddEnvironmentVariable("ESP8266_SDK")
	esp8266corelib.AddInclude("{ESP8266_SDK}", "cores/esp8266", "")
	esp8266corelib.AddInclude("{ESP8266_SDK}", "libraries/Wire", "")
	esp8266corelib.AddInclude("{ESP8266_SDK}", "libraries/EEPROM", "")
	esp8266corelib.SourceFilesFrom("{ESP8266_SDK}", "cores/esp8266", "", ".c", ".cpp", ".S")
	esp8266corelib.SourceFilesFrom("{ESP8266_SDK}", "libraries/Wire", "", ".c", ".cpp")
	esp8266corelib.SourceFilesFrom("{ESP8266_SDK}", "libraries/EEPROM", "", ".c", ".cpp")

	// main library
	mainlib := denv.SetupCppLibProject(mainpkg, name)
	mainlib.AddDependency(esp32corelib)
	mainlib.AddDependency(esp8266corelib)

	// test library
	testlib := denv.SetupCppTestLibProject(mainpkg, name)
	testlib.AddDependencies(ccorepkg.GetTestLib())

	// unittest project
	maintest := denv.SetupCppTestProject(mainpkg, name)
	maintest.AddDependencies(cunittestpkg.GetMainLib())
	maintest.AddDependency(testlib)

	mainpkg.AddMainLib(mainlib)
	mainpkg.AddMainLib(esp32corelib)
	mainpkg.AddMainLib(esp8266corelib)
	mainpkg.AddTestLib(testlib)
	mainpkg.AddUnittest(maintest)
	return mainpkg
}
