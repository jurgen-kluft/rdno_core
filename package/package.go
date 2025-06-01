package rdno_core

import (
	denv "github.com/jurgen-kluft/ccode/denv"
	cunittest "github.com/jurgen-kluft/cunittest/package"
)

// rdno_core is the core package for Arduino projects.
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
	cunittestpkg := cunittest.GetPackage()

	// main package
	mainpkg := denv.NewPackage(repo_path, repo_name)
	mainpkg.AddPackage(cunittestpkg)

	// main library
	mainlib := denv.SetupCppLibProject(mainpkg, name)

	// esp32 core library
	esp32corelib := denv.SetupCppLibProjectForArduino(mainpkg, "esp32-core")
	esp32corelib.ClearIncludes()
	esp32corelib.ClearSourcePaths()
	esp32corelib.AddEnvironmentVariable("ESP_SDK")
	esp32corelib.AddInclude("{ESP_SDK}", "cores/esp32", "")
	esp32corelib.SourceFilesFrom("{ESP_SDK}", "cores/esp32", "")

	// test library
	testlib := denv.SetupCppTestLibProject(mainpkg, name)
	testlib.AddDependencies(cunittestpkg.GetTestLib()...)

	// unittest project
	maintest := denv.SetupCppTestProject(mainpkg, name)
	maintest.AddDependencies(cunittestpkg.GetMainLib()...)
	maintest.AddDependency(testlib)

	mainpkg.AddMainLib(mainlib)
	mainpkg.AddMainLib(esp32corelib)
	mainpkg.AddTestLib(testlib)
	mainpkg.AddUnittest(maintest)
	return mainpkg
}
