using System;
using System.Threading.Tasks;
using Windows.Web.Http;
using Windows.Web.Http.Headers;

namespace HoloLensCommander
{
    public class AppCenterWebRequest
    {
        public async Task<string> HttpWebRequest()
        {
            HttpClient httpClient = new HttpClient();

            var request = new HttpRequestMessage();

            request.Headers.Accept.Add(new HttpMediaTypeWithQualityHeaderValue("application/json"));
            request.Headers.Append("X-API-Token", "9b8e356d29f80de93d88fac0252e48f8fbe22e96");

            Uri uri = new Uri("https://api.appcenter.ms/v0.1/apps");
            request.RequestUri = uri;


            HttpResponseMessage httpResponseMessage = new HttpResponseMessage();
            string httpResponseBody = "";

            try
            {
                httpResponseMessage = await httpClient.SendRequestAsync(request);
                httpResponseMessage.EnsureSuccessStatusCode();
                httpResponseBody = await httpResponseMessage.Content.ReadAsStringAsync();
            }
            catch (Exception ex)
            {
                httpResponseBody = "Error: " + ex.HResult.ToString("X") + " Message: " + ex.Message;
            }

            return httpResponseBody;
        }
    }
}