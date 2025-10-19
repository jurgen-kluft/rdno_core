package rdno_core

import (
	denv "github.com/jurgen-kluft/ccode/denv"
	csdk "github.com/jurgen-kluft/csdk/package"
	cunittest "github.com/jurgen-kluft/cunittest/package"
)

// Core package for Arduino projects, both for ESP32 and ESP8266.
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
	csdkpkg := csdk.GetPackage()
	cunittestpkg := cunittest.GetPackage()

	// main package
	mainpkg := denv.NewPackage(repo_path, repo_name)
	mainpkg.AddPackage(csdkpkg)
	mainpkg.AddPackage(cunittestpkg)

	// esp32 core library
	esp32corelib := denv.SetupCppLibProjectForArduinoEsp32(mainpkg, "esp32-core")
	esp32corelib.ClearIncludes()
	esp32corelib.ClearSourcePaths()
	esp32corelib.AddEnvironmentVariable("ESP32_SDK")
	esp32corelib.AddInclude("{ESP32_SDK}", "cores/esp32", "")
	esp32corelib.AddInclude("{ESP32_SDK}", "libraries/Wire", "src")
	esp32corelib.SourceFilesFrom("{ESP32_SDK}", "cores/esp32", "")
	esp32corelib.SourceFilesFrom("{ESP32_SDK}", "libraries/Wire", "src")

	// esp8266 core library
	esp8266corelib := denv.SetupCppLibProjectForArduinoEsp8266(mainpkg, "esp8266-core")
	esp8266corelib.ClearIncludes()
	esp8266corelib.ClearSourcePaths()
	esp8266corelib.AddEnvironmentVariable("ESP8266_SDK")
	esp8266corelib.AddInclude("{ESP8266_SDK}", "cores/esp8266", "")
	esp8266corelib.AddInclude("{ESP8266_SDK}", "libraries/Wire", "src")
	esp8266corelib.SourceFilesFrom("{ESP8266_SDK}", "cores/esp8266", "")
	esp8266corelib.SourceFilesFrom("{ESP8266_SDK}", "libraries/Wire", "src")

	// main library
	mainlib := denv.SetupCppLibProject(mainpkg, name)
	mainlib.AddDependency(esp32corelib)
	mainlib.AddDependency(esp8266corelib)

	// test library
	testlib := denv.SetupCppTestLibProject(mainpkg, name)

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
