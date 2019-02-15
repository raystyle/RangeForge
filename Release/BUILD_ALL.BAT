SET ROOT_DIR=%~dp0
CD .\Tests

FOR /R %%G in ("*.SDL") DO (
CD "%ROOT_DIR%Tools"
"RangeForge.exe" "%%G" -o "%%G_compiled.exe"
)

CD %ROOT_DIR%